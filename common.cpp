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
  std::string error_to_string(PRL_RESULT result) {
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
    luaX_push(L, error_to_string(result));
    luaX_push(L, result);
  }
}
