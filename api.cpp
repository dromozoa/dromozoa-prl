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

#include <SdkWrap.h>

#include <dromozoa/bind.hpp>

#include "api.hpp"
#include "enum.hpp"
#include "error.hpp"

namespace dromozoa {
  namespace {
    void impl_init_ex(lua_State* L) {
      PRL_RESULT result = PrlApi_InitEx(
          luaL_optinteger(L, 1, PARALLELS_API_VER),
          opt_enum(L, 2, PAM_DESKTOP),
          luaL_optinteger(L, 3, 0),
          luaL_optinteger(L, 4, 0));
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        luaX_push_success(L);
      }
    }

    void impl_deinit(lua_State* L) {
      PRL_RESULT result = PrlApi_Deinit();
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        luaX_push_success(L);
      }
    }
  }

  void initialize_api(lua_State* L) {
    luaX_set_field(L, -1, "init_ex", impl_init_ex);
    luaX_set_field(L, -1, "deinit", impl_deinit);
  }
}
