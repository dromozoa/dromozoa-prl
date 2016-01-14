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
}

#include <SdkWrap.h>

#include <stdio.h>

#include <iostream>

#include "error.hpp"

namespace dromozoa {
  int push_error_string(lua_State* L, PRL_RESULT result) {
    const char* string = 0;
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &string);
    }
    if (string) {
      lua_pushstring(L, string);
    } else {
      if (PRL_SUCCEEDED(result)) {
        lua_pushfstring(L, "PRL_RESULT_DECLARE_SUCCESS(%d)", result);
      } else {
        enum { size = sizeof(PRL_RESULT) * 2 + 1 };
        char buffer[size] = { '\0' };
        snprintf(buffer, size, "%x", result);
        lua_pushfstring(L, "PRL_RESULT_DECLARE_ERROR(%s)", buffer);
      }
    }
    return 1;
  }

  int push_error(lua_State* L, PRL_RESULT result) {
    lua_pushnil(L);
    push_error_string(L, result);
    lua_pushinteger(L, result);
    return 3;
  }

  void print_error(std::ostream& out, PRL_RESULT result) {
    const char* string = 0;
    if (PrlDbg_PrlResultToString) {
      PrlDbg_PrlResultToString(result, &string);
    }
    if (string) {
      out << string;
    } else {
      if (PRL_SUCCEEDED(result)) {
        out << "PRL_RESULT_DECLARE_SUCCESS(" << result << ")";
      } else {
        enum { size = sizeof(PRL_RESULT) * 2 + 1 };
        char buffer[size] = { '\0' };
        snprintf(buffer, size, "%x", result);
        out << "PRL_RESULT_DECLARE_ERROR(" << buffer << ")";
      }
    }
  }
}
