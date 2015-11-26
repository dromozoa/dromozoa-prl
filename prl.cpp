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

#include <iostream>

namespace dromozoa {
  inline void set_field(lua_State* L, const char* key, lua_CFunction value) {
    lua_pushcfunction(L, value);
    lua_setfield(L, -2, key);
  }

  inline int return_boolean(lua_State* L, bool value) {
    lua_pushboolean(L, value);
    return 1;
  }

  inline int return_true(lua_State* L) {
    return return_boolean(L, true);
  }

  inline int return_false(lua_State* L) {
    return return_boolean(L, true);
  }

  inline int return_error(lua_State* L, const char* message, int result) {
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

  inline int open_sdk_wrap(lua_State* L) {
    lua_newtable(L);

    set_field(L, "load", [](lua_State* L){
      const char* name = luaL_checkstring(L, 1);
      bool debug = lua_toboolean(L, 2);
      PRL_RESULT result = SdkWrap_Load(name, debug);
      if (PRL_FAILED(result)) {
        return return_error(L, "could not SdkWrap_Load", result);
      } else {
        return return_true(L);
      }
    });

    set_field(L, "unload", [](lua_State* L){
      PRL_RESULT result = SdkWrap_Unload();
      if (PRL_FAILED(result)) {
        return return_error(L, "could not SdkWrap_Unload", result);
      } else {
        return return_true(L);
      }
    });

    set_field(L, "is_loaded", [](lua_State* L){
      PRL_BOOL result = SdkWrap_IsLoaded();
      lua_pushboolean(L, result);
      return 1;
    });

    set_field(L, "load_lib_from_std_paths", [](lua_State* L) {
      bool debug = lua_toboolean(L, 1);
      PRL_BOOL result = SdkWrap_LoadLibFromStdPaths(debug);
      if (PRL_FAILED(result)) {
        return return_error(L, "could not SdkWrap_LoadLibFromStdPaths", result);
      } else {
        return return_true(L);
      }
    });

    return 1;
  }

  inline int open_api(lua_State* L) {
    lua_newtable(L);

    set_field(L, "deinit", [](lua_State* L) {
      PRL_RESULT result = PrlSdkWrapNamespace::PrlApi_Deinit();
      if (PRL_FAILED(result)) {
        return dromozoa::return_error(L, "could not deinit", result);
      } else {
        return return_true(L);
      }
    });

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
