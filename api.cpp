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

#include <sstream>

#include "common.hpp"

namespace dromozoa {
  namespace {
    void impl_init_ex(lua_State* L) {
      PRL_RESULT result = PrlApi_InitEx(
          luaX_opt_integer<PRL_UINT32>(L, 1, PARALLELS_API_VER),
          luaX_opt_enum(L, 2, PAM_DESKTOP),
          luaX_opt_integer<PRL_UINT32>(L, 3, 0),
          luaX_opt_integer<PRL_UINT32>(L, 4, 0));
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

    void impl_succeeded(lua_State* L) {
      return luaX_push<bool>(L, PRL_SUCCEEDED(luaX_check_integer<PRL_RESULT>(L, 1)));
    }

    void impl_failed(lua_State* L) {
      return luaX_push<bool>(L, PRL_FAILED(luaX_check_integer<PRL_RESULT>(L, 1)));
    }

    void impl_result_to_string(lua_State* L) {
      return luaX_push(L, result_to_string(luaX_check_integer<PRL_RESULT>(L, 1)));
    }
  }

  std::string result_to_string(PRL_RESULT result) {
    const char* string = 0;
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &string);
    }
    if (string) {
      return string;
    } else {
      std::ostringstream out;
      if (PRL_SUCCEEDED(result)) {
        out << "PRL_RESULT_DECLARE_SUCCESS(" << result << ")";
      } else {
        out << "PRL_RESULT_DECLARE_ERROR(" << std::hex << result << ")";
      }
      return out.str();
    }
  }

  void push_error(lua_State* L, PRL_RESULT result) {
    luaX_push(L, luaX_nil);
    luaX_push(L, result_to_string(result));
    luaX_push(L, result);
  }

  void initialize_api(lua_State* L) {
    luaX_set_field(L, -1, "init_ex", impl_init_ex);
    luaX_set_field(L, -1, "deinit", impl_deinit);
    luaX_set_field(L, -1, "succeeded", impl_succeeded);
    luaX_set_field(L, -1, "failed", impl_failed);
    luaX_set_field(L, -1, "result_to_string", impl_result_to_string);
  }
}
