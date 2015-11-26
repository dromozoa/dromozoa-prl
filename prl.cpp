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

#include <cstddef>
#include <limits>
#include <iostream>

#define DROMOZOA_SET_FIELD(L, value) \
  dromozoa::set_field(L, #value, (value))

namespace dromozoa {
  inline void set_field(lua_State* L, const char* key, lua_Integer value) {
    lua_pushinteger(L, value);
    lua_setfield(L, -2, key);
  }

  inline void set_field(lua_State* L, const char* key, lua_CFunction value) {
    lua_pushcfunction(L, value);
    lua_setfield(L, -2, key);
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

  inline void save_metatable_index(lua_State* L, const char* name) {
    luaL_newmetatable(L, name);
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    lua_pushstring(L, name);
    lua_setfield(L, -2, "name");
    lua_pop(L, 1);
  }

  inline int result_error(lua_State* L, const char* message, int result) {
    const char* reason = 0;
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &reason);
    }
    lua_pushnil(L);
    if (reason) {
      lua_pushfstring(L, "%s: %s", message, reason);
    } else {
      lua_pushfstring(L, "%s: %d", message, result);
    }
    return 2;
  }

  inline int result_handle(lua_State* L, PRL_HANDLE result) {

    std::cerr << "handle: " << result << "\n";

    if (result == PRL_INVALID_HANDLE) {
      return 0;
    }

    const char* name = 0;

    PRL_HANDLE_TYPE type = PHT_ERROR;
    if (!PRL_FAILED(PrlHandle_GetType(result, &type))) {
      switch (type) {
        case PHT_JOB:
          name = "dromozoa.prl.api.job";
          break;
        case PHT_SERVER:
          name = "dromozoa.prl.api.server";
          break;
        default:
          name = "dromozoa.prl.api.handle";
          break;
      }
    }

    std::cerr << lua_gettop(L) << std::endl;

    void* data = lua_newuserdata(L, sizeof(PRL_HANDLE));
    *static_cast<PRL_HANDLE*>(data) = result;
    // lua_pushlightuserdata(L, result);
    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);

    std::cerr << lua_gettop(L) << std::endl;

    return 1;
  }

  inline int result_error(lua_State* L, PRL_RESULT result) {
    const char* message = "error";
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &message);
    }
    lua_pushnil(L);
    lua_pushstring(L, message);
    lua_pushinteger(L, result);
    return 3;
  }

  inline int ret(lua_State* L, PRL_RESULT result) {
    if (PRL_FAILED(result)) {
      return result_error(L, result);
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  }

  inline int ret(lua_State* L, PRL_HANDLE result) {
    if (result == PRL_INVALID_HANDLE) {
      return result_error(L, PRL_ERR_INVALID_HANDLE);
    } else {
      return result_handle(L, result);
    }
  }

  inline int ret(lua_State* L, PRL_RESULT result, const PRL_RESULT* code) {
    if (PRL_FAILED(result)) {
      return result_error(L, result);
    } else {
      lua_pushstring(L, prl_result_to_string(*code));
      lua_pushinteger(L, *code);
      return 2;
    }
  }

  inline int ret(lua_State* L, PRL_RESULT result, const PRL_HANDLE* handle) {
    if (PRL_FAILED(result)) {
      return result_error(L, result);
    } else {
      return result_handle(L, *handle);
    }
  }

  inline int ret(lua_State* L, PRL_RESULT result, const PRL_HANDLE_TYPE* type) {
    if (PRL_FAILED(result)) {
      return result_error(L, result);
    } else {
      lua_pushstring(L, handle_type_to_string(*type));
      lua_pushinteger(L, *type);
      return 2;
    }
  }

  inline int open_sdk_wrap(lua_State* L) {
    lua_newtable(L);

    set_field(L, "load", [](lua_State* L) {
      return ret(L,
          SdkWrap_Load(
              luaL_checkstring(L, 1),
              lua_toboolean(L, 2)));
    });

    set_field(L, "load_lib_from_std_paths", [](lua_State* L) {
      return ret(L,
          SdkWrap_LoadLibFromStdPaths(
              lua_toboolean(L, 1)));
    });

    set_field(L, "unload", [](lua_State* L) {
      return ret(L, SdkWrap_Unload());
    });

    set_field(L, "is_loaded", [](lua_State* L) {
      lua_pushboolean(L, SdkWrap_IsLoaded());
      return 1;
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
      return ret(L,
          PrlHandle_GetType(
              get_handle(L, 1),
              &type),
          &type);
    });

    save_metatable_index(L, "dromozoa.prl.api.handle");

    return 1;
  }

  inline int open_api_job(lua_State* L) {
    lua_newtable(L);

    set_field(L, "wait", [](lua_State* L) {
      return ret(L,
          PrlJob_Wait(
              get_handle(L, 1),
              luaL_optinteger(L, 2, std::numeric_limits<std::uint32_t>::max())));
    });

    set_field(L, "get_result", [](lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      return ret(L,
          PrlJob_GetResult(
              get_handle(L, 1),
              &handle),
          &handle);
    });

    set_field(L, "get_ret_code", [](lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      return ret(L,
          PrlJob_GetRetCode(
              get_handle(L, 1),
              &code),
          &code);
    });

    save_metatable_index(L, "dromozoa.prl.api.job");
    luaL_getmetatable(L, "dromozoa.prl.api.handle");
    lua_setmetatable(L, -2);

    return 1;
  }

  inline int open_api_result(lua_State* L) {
    lua_newtable(L);

    set_field(L, "get_params_count", [](lua_State* L) {
      PRL_UINT32 count = 0;
      PRL_RESULT result = PrlResult_GetParamsCount(
          get_handle(L, 1),
          &count);
      if (PRL_SUCCEEDED(result)) {
        lua_pushinteger(L, count);
        return 1;
      }
      return ret(L, result);
    });

    save_metatable_index(L, "dromozoa.prl.api.result");
    luaL_getmetatable(L, "dromozoa.prl.api.handle");
    lua_setmetatable(L, -2);

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
              static_cast<PRL_SECURITY_LEVEL>(luaL_optinteger(L, 4, PSL_NORMAL_SECURITY))));
    });

    set_field(L, "logoff", [](lua_State* L) {
      return ret(L, PrlSrv_Logoff( get_handle(L, 1)));
    });

    save_metatable_index(L, "dromozoa.prl.api.server");
    luaL_getmetatable(L, "dromozoa.prl.api.handle");
    lua_setmetatable(L, -2);

    return 1;
  }

  inline int open_api(lua_State* L) {
    lua_newtable(L);

    set_field(L, "init", [](lua_State* L) {
      return ret(L, PrlApi_Init(luaL_optinteger(L, 1, PARALLELS_API_VER)));
    });

    set_field(L, "init_ex", [](lua_State* L) {
      return ret(L,
          PrlApi_InitEx(
              luaL_optinteger(L, 1, PARALLELS_API_VER),
              static_cast<PRL_APPLICATION_MODE>(luaL_optinteger(L, 2, PAM_DESKTOP)),
              luaL_optinteger(L, 3, 0),
              luaL_optinteger(L, 4, 0)));
    });

    set_field(L, "deinit", [](lua_State* L) {
      return ret(L, PrlApi_Deinit());
    });

    open_api_handle(L);
    lua_setfield(L, -2, "handle");

    open_api_job(L);
    lua_setfield(L, -2, "job");

    open_api_result(L);
    lua_setfield(L, -2, "result");

    open_api_server(L);
    lua_setfield(L, -2, "server");

    set_field(L, "API_VER", PARALLELS_API_VER);

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

    return 1;
  }
}

#undef DROMOZOA_SET_ENUM

extern "C" int luaopen_dromozoa_prl(lua_State* L) {
  lua_newtable(L);
  dromozoa::open_sdk_wrap(L);
  lua_setfield(L, -2, "sdk_wrap");
  dromozoa::open_api(L);
  lua_setfield(L, -2, "api");
  return 1;
}
