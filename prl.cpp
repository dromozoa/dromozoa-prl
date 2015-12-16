// Copyright (C) 2015 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-prl.
//
// dromozoa-prl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-prl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace dromozoa {
  lua_Integer log_level = 0;

  inline void set_field(lua_State* L, const char* key, lua_Integer value) {
    lua_pushinteger(L, value);
    lua_setfield(L, -2, key);
  }

  inline void set_field(lua_State* L, const char* key, lua_CFunction value) {
    lua_pushcfunction(L, value);
    lua_setfield(L, -2, key);
  }

  template <class T>
  inline T get_enum(lua_State* L, int n) {
    return static_cast<T>(luaL_checkinteger(L, n));
  }

  template <class T>
  inline T opt_enum(lua_State* L, int n, T d) {
    return static_cast<T>(luaL_optinteger(L, n, d));
  }

  inline PRL_HANDLE get_handle(lua_State* L, int n) {
    PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, n));
    if (!data) {
      luaL_argerror(L, n, "userdata expected, got nil");
      return PRL_INVALID_HANDLE;
    }
    return *data;
  }

  inline void set_invalid_handle(lua_State* L, int n) {
    PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, n));
    if (!data) {
      luaL_argerror(L, n, "userdata expected, got nil");
      return;
    }
    *data = PRL_INVALID_HANDLE;
  }

  inline lua_Integer get_handle_address(PRL_HANDLE handle) {
    return reinterpret_cast<lua_Integer>(handle);
  }

  inline PRL_RESULT free_handle(PRL_HANDLE handle) {
    if (PrlHandle_Free) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      PrlHandle_GetType(handle, &type);

      PRL_RESULT result = PrlHandle_Free(handle);
      if (PRL_SUCCEEDED(result)) {
        if (log_level > 2) {
          std::cerr << "[dromozoa-prl] detach handle " << get_handle_address(handle) << " (" << handle_type_to_string(type) << ")" << std::endl;
        }
      }
      return result;
    } else {
      return PRL_ERR_UNINITIALIZED;
    }
  }

  inline std::string result_to_str(PRL_RESULT result) {
    const char* string = 0;
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &string);
    }
    if (string) {
      return string;
    }
    std::ostringstream out;
    if (PRL_SUCCEEDED(result)) {
      out << "PRL_RESULT_DECLARE_SUCCESS(" << result << ")";
    } else {
      out << "PRL_RESULT_DECLARE_ERROR(" << std::hex << (result - 0x80000000) << ")";
    }
    return out.str();
  }

  inline int ret_error(lua_State* L, PRL_RESULT result) {
    lua_pushnil(L);
    lua_pushstring(L, result_to_str(result).c_str());
    lua_pushinteger(L, result);
    return 3;
  }

  inline int ret_handle(lua_State* L, PRL_HANDLE handle) {
    PRL_HANDLE_TYPE type = PHT_ERROR;
    PrlHandle_GetType(handle, &type);

    const char* name = "dromozoa.prl.handle";
    switch (type) {
      case PHT_JOB:
        name = "dromozoa.prl.job";
        break;
      case PHT_RESULT:
        name = "dromozoa.prl.result";
        break;
      case PHT_SERVER:
        name = "dromozoa.prl.server";
        break;
      case PHT_VIRTUAL_MACHINE:
        name = "dromozoa.prl.virtual_machine";
        break;
      default:
        break;
    }

    PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_newuserdata(L, sizeof(PRL_HANDLE)));
    *data = handle;
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);

    if (log_level > 2) {
      std::cerr << "[dromozoa-prl] attach handle " << get_handle_address(handle) << " (" << handle_type_to_string(type) << ")" << std::endl;
    }

    return 1;
  }

  inline int ret(lua_State* L, PRL_RESULT result) {
    if (PRL_FAILED(result)) {
      return ret_error(L, result);
    } else {
      if (lua_isuserdata(L, 1)) {
        lua_pushvalue(L, 1);
      } else {
        lua_pushboolean(L, true);
      }
      return 1;
    }
  }

  inline int ret(lua_State* L, PRL_HANDLE result) {
    if (result == PRL_INVALID_HANDLE) {
      return ret_error(L, PRL_ERR_INVALID_HANDLE);
    } else {
      return ret_handle(L, result);
    }
  }

  inline int ret(lua_State* L, PRL_RESULT result, const PRL_RESULT* code) {
    if (PRL_FAILED(result)) {
      return ret_error(L, result);
    } else {
      lua_pushstring(L, result_to_str(*code).c_str());
      lua_pushinteger(L, *code);
      return 2;
    }
  }

  inline int ret(lua_State* L, PRL_RESULT result, const PRL_HANDLE* handle) {
    if (PRL_FAILED(result)) {
      return ret_error(L, result);
    } else {
      return ret_handle(L, *handle);
    }
  }

  inline int ret(lua_State* L, PRL_RESULT result, const PRL_HANDLE_TYPE* type) {
    if (PRL_FAILED(result)) {
      return ret_error(L, result);
    } else {
      lua_pushstring(L, handle_type_to_string(*type));
      lua_pushinteger(L, *type);
      return 2;
    }
  }

  inline void prototype(lua_State* L, const char* name) {
    luaL_newmetatable(L, name);
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    set_field(L, "__gc", [](lua_State* L) {
      PRL_HANDLE handle = get_handle(L, 1);
      if (handle != PRL_INVALID_HANDLE) {
        lua_Integer address = get_handle_address(handle);
        if (log_level > 1) {
          std::cerr << "[dromozoa-prl] handle " << address << " detected" << std::endl;
        }
        PRL_RESULT result = free_handle(handle);
        if (PRL_SUCCEEDED(result)) {
          set_invalid_handle(L, 1);
        } else if (log_level > 0) {
          std::cerr << "[dromozoa-prl] cannot free handle " << address << ": " << result_to_str(result) << std::endl;
        }
      }
      return 0;
    });
    lua_pop(L, 1);
  }

  inline void inherit(lua_State* L, const char* name) {
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
  }

  inline int open_sdk_wrap(lua_State* L) {
    lua_newtable(L);

    set_field(L, "is_loaded", [](lua_State* L) {
      lua_pushboolean(L, SdkWrap_IsLoaded());
      return 1;
    });

    set_field(L, "load", [](lua_State* L) {
      return ret(L, SdkWrap_Load(luaL_checkstring(L, 1), lua_toboolean(L, 2)));
    });

    set_field(L, "load_lib_from_std_paths", [](lua_State* L) {
      return ret(L, SdkWrap_LoadLibFromStdPaths(lua_toboolean(L, 1)));
    });

    set_field(L, "unload", [](lua_State* L) {
      return ret(L, SdkWrap_Unload());
    });

    return 1;
  }

  inline int open_handle(lua_State* L) {
    lua_newtable(L);

    set_field(L, "free", [](lua_State* L) {
      PRL_RESULT result = free_handle(get_handle(L, 1));
      if (PRL_SUCCEEDED(result)) {
        set_invalid_handle(L, 1);
      }
      return ret(L, result);
    });

    set_field(L, "get_type", [](lua_State* L) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      return ret(L, PrlHandle_GetType(get_handle(L, 1), &type), &type);
    });

    set_field(L, "get_address", [](lua_State* L) {
      lua_pushinteger(L, get_handle_address(get_handle(L, 1)));
      return 1;
    });

    prototype(L, "dromozoa.prl.handle");

    return 1;
  }

  inline int open_handle_job(lua_State* L) {
    lua_newtable(L);

    set_field(L, "get_result", [](lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      return ret(L, PrlJob_GetResult(get_handle(L, 1), &handle), &handle);
    });

    set_field(L, "get_ret_code", [](lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      return ret(L, PrlJob_GetRetCode(get_handle(L, 1), &code), &code);
    });

    set_field(L, "wait", [](lua_State* L) {
      return ret(L,
          PrlJob_Wait(
              get_handle(L, 1),
              luaL_optinteger(L, 2, std::numeric_limits<PRL_UINT32>::max())));
    });

    set_field(L, "check_ret_code", [](lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      PRL_RESULT result = PrlJob_GetRetCode(get_handle(L, 1), &code);
      if (PRL_SUCCEEDED(result)) {
        if (PRL_SUCCEEDED(code)) {
          lua_pushvalue(L, 1);
          return 1;
        } else {
          return luaL_error(L, "%s", result_to_str(code).c_str());
        }
      }
      return luaL_error(L, "%s", result_to_str(result).c_str());
    });

    set_field(L, "get_result_and_free", [](lua_State* L) {
      PRL_HANDLE self = get_handle(L, 1);
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlJob_GetResult(self, &handle);
      if (PRL_SUCCEEDED(result)) {
        PRL_RESULT result = free_handle(self);
        if (PRL_SUCCEEDED(result)) {
          set_invalid_handle(L, 1);
          return ret(L, result, &handle);
        }
      }
      return luaL_error(L, "%s", result_to_str(result).c_str());
    });

    prototype(L, "dromozoa.prl.job");
    inherit(L, "dromozoa.prl.handle");

    return 1;
  }

  inline int open_handle_result(lua_State* L) {
    lua_newtable(L);

    set_field(L, "get_param_by_index", [](lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      return ret(L,
          PrlResult_GetParamByIndex(
              get_handle(L, 1),
              luaL_checkinteger(L, 2),
              &handle),
          &handle);
    });

    set_field(L, "get_params_count", [](lua_State* L) {
      PRL_UINT32 count = 0;
      PRL_RESULT result = PrlResult_GetParamsCount(get_handle(L, 1), &count);
      if (PRL_SUCCEEDED(result)) {
        lua_pushinteger(L, count);
        return 1;
      }
      return ret(L, result);
    });

    prototype(L, "dromozoa.prl.result");
    inherit(L, "dromozoa.prl.handle");

    return 1;
  }

  inline int open_handle_server(lua_State* L) {
    lua_newtable(L);

    set_field(L, "create", [](lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      return ret(L, PrlSrv_Create(&handle), &handle);
    });

    set_field(L, "get_vm_list", [](lua_State* L) {
      return ret(L, PrlSrv_GetVmList(get_handle(L, 1)));
    });

    set_field(L, "login_local", [](lua_State* L) {
      return ret(L,
          PrlSrv_LoginLocal(
              get_handle(L, 1),
              lua_tostring(L, 2),
              luaL_optinteger(L, 3, 0),
              opt_enum(L, 4, PSL_NORMAL_SECURITY)));
    });

    set_field(L, "logoff", [](lua_State* L) {
      return ret(L, PrlSrv_Logoff(get_handle(L, 1)));
    });

    prototype(L, "dromozoa.prl.server");
    inherit(L, "dromozoa.prl.handle");

    return 1;
  }

  inline int open_handle_vm_configuration(lua_State* L) {
    lua_newtable(L);

    set_field(L, "get_name", [](lua_State* L) {
      PRL_HANDLE handle = get_handle(L, 1);
      PRL_UINT32 size = 0;
      PRL_RESULT result = PrlVmCfg_GetName(handle, 0, &size);
      if (PRL_SUCCEEDED(result)) {
        std::vector<char> buffer(size);
        result = PrlVmCfg_GetName(handle, buffer.data(), &size);
        if (PRL_SUCCEEDED(result)) {
          lua_pushstring(L, buffer.data());
          return 1;
        }
      }
      return ret(L, result);
    });

    prototype(L, "dromozoa.prl.vm_configuration");
    inherit(L, "dromozoa.prl.handle");

    return 1;
  }

  inline int open_handle_virtual_machine(lua_State* L) {
    lua_newtable(L);

    set_field(L, "connect_to_vm", [](lua_State* L) {
      return ret(L,
          PrlDevDisplay_ConnectToVm(
              get_handle(L, 1),
              opt_enum(L, 2, PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION)));
    });

    set_field(L, "disconnect_from_vm", [](lua_State* L) {
      return ret(L, PrlDevDisplay_DisconnectFromVm( get_handle(L, 1)));
    });

    set_field(L, "get_config", [](lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      return ret(L, PrlVm_GetConfig(get_handle(L, 1), &handle), &handle);
    });

    set_field(L, "send_key_event_ex", [](lua_State* L) {
      return ret(L,
          PrlDevKeyboard_SendKeyEventEx(
              get_handle(L, 1),
              get_enum<PRL_KEY>(L, 2),
              get_enum<PRL_KEY_EVENT>(L, 3)));
    });

    set_field(L, "send_key_pressed_and_released", [](lua_State* L) {
      return ret(L,
          PrlDevKeyboard_SendKeyPressedAndReleased(
              get_handle(L, 1),
              get_enum<PRL_KEY>(L, 2)));
    });

    prototype(L, "dromozoa.prl.virtual_machine");
    inherit(L, "dromozoa.prl.vm_configuration");

    return 1;
  }

  inline int open_key(lua_State* L) {
    lua_newtable(L);
#include "key.hpp"
    return 1;
  }

  inline void initialize_core(lua_State* L) {
    set_field(L, "deinit", [](lua_State* L) {
      return ret(L, PrlApi_Deinit());
    });

    set_field(L, "init_ex", [](lua_State* L) {
      return ret(L,
          PrlApi_InitEx(
              luaL_optinteger(L, 1, PARALLELS_API_VER),
              opt_enum(L, 2, PAM_DESKTOP),
              luaL_optinteger(L, 3, 0),
              luaL_optinteger(L, 4, 0)));
    });

    set_field(L, "set_log_level", [](lua_State* L) {
      log_level = luaL_checkinteger(L, 1);
      return 0;
    });

    set_field(L, "get_log_level", [](lua_State* L) {
      lua_pushinteger(L, log_level);
      return 1;
    });

    open_handle(L);
    lua_setfield(L, -2, "handle");

    open_handle_job(L);
    lua_setfield(L, -2, "job");

    open_handle_result(L);
    lua_setfield(L, -2, "result");

    open_handle_server(L);
    lua_setfield(L, -2, "server");

    open_handle_vm_configuration(L);
    lua_setfield(L, -2, "vm_configuration");

    open_handle_virtual_machine(L);
    lua_setfield(L, -2, "virtual_machine");

    open_key(L);
    lua_setfield(L, -2, "key");
  }

  inline void initialize_enum(lua_State* L) {
    set_field(L, "API_VER", PARALLELS_API_VER);

#define DROMOZOA_SET_FIELD(L, value) \
  dromozoa::set_field(L, #value, (value))

    // PRL_API_COMMAND_FLAGS
    DROMOZOA_SET_FIELD(L, PACF_NORMAL_SECURITY);
    DROMOZOA_SET_FIELD(L, PACF_HIGH_SECURITY);
    DROMOZOA_SET_FIELD(L, PACF_NON_INTERACTIVE_MODE);
    DROMOZOA_SET_FIELD(L, PACF_CANCEL_TASK_ON_END_SESSION);

    // PRL_API_INIT_FLAGS
    DROMOZOA_SET_FIELD(L, PAIF_USE_GRAPHIC_MODE);
    DROMOZOA_SET_FIELD(L, PAIF_INIT_AS_APPSTORE_CLIENT);

    // PRL_APPLICATION_MODE
    DROMOZOA_SET_FIELD(L, PAM_UNKNOWN);
    DROMOZOA_SET_FIELD(L, PAM_SERVER);
    DROMOZOA_SET_FIELD(L, PAM_DESKTOP_MAC);
    DROMOZOA_SET_FIELD(L, PAM_WORKSTATION_EXTREME);
    DROMOZOA_SET_FIELD(L, PAM_PLAYER);
    DROMOZOA_SET_FIELD(L, PAM_DESKTOP_STM);
    DROMOZOA_SET_FIELD(L, PAM_DESKTOP_WL);
    DROMOZOA_SET_FIELD(L, PAM_MOBILE);
    DROMOZOA_SET_FIELD(L, PAM_DESKTOP_STM_OBSOLETE);
    DROMOZOA_SET_FIELD(L, PAM_DESKTOP);
    DROMOZOA_SET_FIELD(L, PAM_WORKSTATION);
    DROMOZOA_SET_FIELD(L, PAM_STM);

    // PRL_DISPLAY_CODEC_TYPE
    DROMOZOA_SET_FIELD(L, PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION);
    DROMOZOA_SET_FIELD(L, PDCT_HIGH_QUALITY_WITH_COMPRESSION);
    DROMOZOA_SET_FIELD(L, PDCT_MEDIUM_QUALITY_WITHOUT_COMPRESSION);
    DROMOZOA_SET_FIELD(L, PDCT_MEDIUM_QUALITY_WITH_COMPRESSION);
    DROMOZOA_SET_FIELD(L, PDCT_LOW_QUALITY_WITHOUT_COMPRESSION);
    DROMOZOA_SET_FIELD(L, PDCT_LOW_QUALITY_WITH_COMPRESSION);

    // PRL_KEY_EVENT
    DROMOZOA_SET_FIELD(L, PKE_PRESS);
    DROMOZOA_SET_FIELD(L, PKE_RELEASE);

#undef DROMOZOA_SET_FIELD
  }
}

extern "C" int luaopen_dromozoa_prl(lua_State* L) {
  lua_newtable(L);

  dromozoa::open_sdk_wrap(L);
  lua_setfield(L, -2, "sdk_wrap");

  dromozoa::initialize_core(L);
  dromozoa::initialize_enum(L);

  return 1;
}
