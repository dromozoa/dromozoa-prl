/*
 * Copyright (C) 2015 Tomoyuki Fujimori <moyu@dromozoa.com>
 *
 * This file is part of dromozoa-prl.
 *
 * dromozoa-prl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dromozoa-prl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.
 */

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <limits>
#include <vector>

namespace dromozoa {
  inline void set_field(lua_State* L, const char* key, lua_Integer value) {
    lua_pushinteger(L, value);
    lua_setfield(L, -2, key);
  }

  inline void set_field(lua_State* L, const char* key, lua_CFunction value) {
    lua_pushcfunction(L, value);
    lua_setfield(L, -2, key);
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

  inline void set_handle(lua_State* L, int n, PRL_HANDLE handle) {
    PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, n));
    if (!data) {
      luaL_argerror(L, n, "userdata expected, got nil");
      return;
    }
    *data = handle;
  }

  inline int ret_error(lua_State* L, PRL_RESULT result) {
    const char* message = "error";
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &message);
    }
    lua_pushnil(L);
    lua_pushstring(L, message);
    lua_pushinteger(L, result);
    return 3;
  }

  inline int ret_handle(lua_State* L, PRL_HANDLE handle) {
    const char* name = "dromozoa.prl.api.handle";
    PRL_HANDLE_TYPE type;
    if (PRL_SUCCEEDED(PrlHandle_GetType(handle, &type))) {
      switch (type) {
        case PHT_JOB:
          name = "dromozoa.prl.api.job";
          break;
        case PHT_RESULT:
          name = "dromozoa.prl.api.result";
          break;
        case PHT_SERVER:
          name = "dromozoa.prl.api.server";
          break;
        case PHT_VIRTUAL_MACHINE:
          name = "dromozoa.prl.api.virtual_machine";
          break;
        default:
          break;
      }
    }
    PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_newuserdata(L, sizeof(PRL_HANDLE)));
    *data = handle;
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
    return 1;
  }

  inline int ret(lua_State* L, PRL_RESULT result) {
    if (PRL_FAILED(result)) {
      return ret_error(L, result);
    } else {
      lua_pushboolean(L, true);
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
      lua_pushstring(L, prl_result_to_string(*code));
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

  inline int open_api_handle(lua_State* L) {
    lua_newtable(L);

    set_field(L, "free", [](lua_State* L) {
      PRL_RESULT result = PrlHandle_Free(get_handle(L, 1));
      if (PRL_SUCCEEDED(result)) {
        set_handle(L, 1, PRL_INVALID_HANDLE);
      }
      return ret(L, result);
    });

    set_field(L, "get_type", [](lua_State* L) {
      PRL_HANDLE_TYPE type = PHT_ERROR;
      return ret(L, PrlHandle_GetType(get_handle(L, 1), &type), &type);
    });

    prototype(L, "dromozoa.prl.api.handle");

    return 1;
  }

  inline int open_api_job(lua_State* L) {
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

    prototype(L, "dromozoa.prl.api.job");
    inherit(L, "dromozoa.prl.api.handle");

    return 1;
  }

  inline int open_api_result(lua_State* L) {
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

    prototype(L, "dromozoa.prl.api.result");
    inherit(L, "dromozoa.prl.api.handle");

    return 1;
  }

  inline int open_api_server(lua_State* L) {
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

    prototype(L, "dromozoa.prl.api.server");
    inherit(L, "dromozoa.prl.api.handle");

    return 1;
  }

  inline int open_api_vm_configuration(lua_State* L) {
    lua_newtable(L);

    set_field(L, "get_name", [](lua_State* L) {
      PRL_HANDLE handle = get_handle(L, 1);
      PRL_UINT32 size = 0;
      PRL_RESULT result = PrlVmCfg_GetName(handle, 0, &size);
      if (PRL_SUCCEEDED(result)) {
        std::vector<char> buffer(size);
        result = PrlVmCfg_GetName(handle, buffer.data(), &size);
        if (PRL_SUCCEEDED(result)) {
          lua_pushlstring(L, buffer.data(), size - 1);
          return 1;
        }
      }
      return ret(L, result);
    });

    prototype(L, "dromozoa.prl.api.vm_configuration");
    inherit(L, "dromozoa.prl.api.handle");

    return 1;
  }

  inline int open_api_virtual_machine(lua_State* L) {
    lua_newtable(L);

    set_field(L, "get_config", [](lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      return ret(L, PrlVm_GetConfig(get_handle(L, 1), &handle), &handle);
    });

    set_field(L, "send_key_event_ex", [](lua_State* L) {
      return ret(L,
          PrlDevKeyboard_SendKeyEventEx(
              get_handle(L, 1),
              static_cast<PRL_KEY>(luaL_checkinteger(L, 2)),
              static_cast<PRL_KEY_EVENT>(luaL_checkinteger(L, 3))));
    });

    set_field(L, "send_key_pressed_and_released", [](lua_State* L) {
      return ret(L,
          PrlDevKeyboard_SendKeyPressedAndReleased(
              get_handle(L, 1),
              static_cast<PRL_KEY>(luaL_checkinteger(L, 2))));
    });

    prototype(L, "dromozoa.prl.api.virtual_machine");
    inherit(L, "dromozoa.prl.api.vm_configuration");

    return 1;
  }

  inline int open_api_key(lua_State* L) {
    lua_newtable(L);
#include "prl_key.hpp"
    return 1;
  }

  inline int open_api(lua_State* L) {
    lua_newtable(L);

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

    open_api_handle(L);
    lua_setfield(L, -2, "handle");

    open_api_job(L);
    lua_setfield(L, -2, "job");

    open_api_result(L);
    lua_setfield(L, -2, "result");

    open_api_server(L);
    lua_setfield(L, -2, "server");

    open_api_vm_configuration(L);
    lua_setfield(L, -2, "vm_configuration");

    open_api_virtual_machine(L);
    lua_setfield(L, -2, "virtual_machine");

    open_api_key(L);
    lua_setfield(L, -2, "key");

    set_field(L, "API_VER", PARALLELS_API_VER);

#define DROMOZOA_SET_FIELD(L, value) \
  dromozoa::set_field(L, #value, (value))

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

    // PRL_API_INIT_FLAGS
    DROMOZOA_SET_FIELD(L, PAIF_USE_GRAPHIC_MODE);
    DROMOZOA_SET_FIELD(L, PAIF_INIT_AS_APPSTORE_CLIENT);

    // PRL_API_COMMAND_FLAGS
    DROMOZOA_SET_FIELD(L, PACF_NORMAL_SECURITY);
    DROMOZOA_SET_FIELD(L, PACF_HIGH_SECURITY);
    DROMOZOA_SET_FIELD(L, PACF_NON_INTERACTIVE_MODE);
    DROMOZOA_SET_FIELD(L, PACF_CANCEL_TASK_ON_END_SESSION);

    // PRL_KEY_EVENT
    DROMOZOA_SET_FIELD(L, PKE_PRESS);
    DROMOZOA_SET_FIELD(L, PKE_RELEASE);

#undef DROMOZOA_SET_FIELD

    return 1;
  }
}

extern "C" int luaopen_dromozoa_prl(lua_State* L) {
  lua_newtable(L);

  dromozoa::open_sdk_wrap(L);
  lua_setfield(L, -2, "sdk_wrap");
  dromozoa::open_api(L);
  lua_setfield(L, -2, "api");

  return 1;
}
