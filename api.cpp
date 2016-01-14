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

#include "dromozoa/bind.hpp"

#include "api.hpp"
#include "enum.hpp"
#include "error.hpp"

namespace dromozoa {
  using bind::function;
  using bind::push_success;

  namespace {
    int impl_init_ex(lua_State* L) {
      PRL_RESULT result = PrlApi_InitEx(
          luaL_optinteger(L, 1, PARALLELS_API_VER),
          opt_enum(L, 2, PAM_DESKTOP),
          luaL_optinteger(L, 3, 0),
          luaL_optinteger(L, 4, 0));
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return push_success(L);
      }
    }

    int impl_deinit(lua_State* L) {
      PRL_RESULT result = PrlApi_Deinit();
      if (PRL_FAILED(result)) {
        return push_error(L, result);
      } else {
        return push_success(L);
      }
    }
  }

  void initialize_api(lua_State* L) {
    function<impl_init_ex>::set_field(L, "init_ex");
    function<impl_deinit>::set_field(L, "deinit");
  }
}
