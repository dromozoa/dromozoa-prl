// Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
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
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <limits>

#include "dromozoa/bind.hpp"

#include "error.hpp"
#include "handle.hpp"
#include "result.hpp"

namespace dromozoa {
  using bind::function;
  using bind::push_success;

  namespace {
    int impl_get_result(lua_State* L) {
      PRL_HANDLE handle = PRL_INVALID_HANDLE;
      PRL_RESULT result = PrlJob_GetResult(get_handle(L, 1), &handle);
      return push_result(L, result, handle);
    }

    int impl_get_ret_code(lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      PRL_RESULT result = PrlJob_GetRetCode(get_handle(L, 1), &code);
      return push_result(L, result, code);
    }

    int impl_wait(lua_State* L) {
      return push_result(L, PrlJob_Wait(get_handle(L, 1), luaL_optinteger(L, 2, std::numeric_limits<PRL_UINT32>::max())));
    }

    int impl_check_ret_code(lua_State* L) {
      PRL_RESULT code = PRL_ERR_SUCCESS;
      PRL_RESULT result = PrlJob_GetRetCode(get_handle(L, 1), &code);
      if (PRL_SUCCEEDED(result)) {
        if (PRL_SUCCEEDED(code)) {
          return push_success(L);
        } else {
          push_error_string(L, code);
          return lua_error(L);
        }
      } else {
        push_error_string(L, result);
        return lua_error(L);
      }
    }

    int impl_get_result_and_free(lua_State* L) {
      if (PRL_HANDLE* data = static_cast<PRL_HANDLE*>(lua_touserdata(L, 1))) {
        PRL_HANDLE handle = PRL_INVALID_HANDLE;
        PRL_RESULT result = PrlJob_GetResult(*data, &handle);
        if (PRL_SUCCEEDED(result)) {
          PRL_RESULT result = free_handle(*data);
          if (PRL_SUCCEEDED(result)) {
            *data = PRL_INVALID_HANDLE;
            new_handle(L, handle);
            return 1;
          } else {
            return push_error(L, result);
          }
        } else {
          return push_error(L, result);
        }
      } else {
        return push_error(L, PRL_ERR_INVALID_HANDLE);
      }
    }
  }

  int open_handle_job(lua_State* L) {
    lua_newtable(L);
    function<impl_get_result>::set_field(L, "get_result");
    function<impl_get_ret_code>::set_field(L, "get_ret_code");
    function<impl_wait>::set_field(L, "wait");
    function<impl_check_ret_code>::set_field(L, "check_ret_code");
    function<impl_get_result_and_free>::set_field(L, "get_result_and_free");

    luaL_getmetatable(L, "dromozoa.prl.handle");
    lua_setmetatable(L, -2);

    luaL_newmetatable(L, "dromozoa.prl.job");
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    function<gc_handle>::set_field(L, "__gc");
    lua_pop(L, 1);

    return 1;
  }
}
