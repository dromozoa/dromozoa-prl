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

#ifndef DROMOZOA_COMMON_HPP
#define DROMOZOA_COMMON_HPP

#include <SdkWrap.h>

#include <dromozoa/bind.hpp>

namespace dromozoa {
  template <class T>
  inline T check_enum(lua_State* L, int n) {
    return static_cast<T>(luaL_checkinteger(L, n));
  }

  template <class T>
  inline T opt_enum(lua_State* L, int n, T d) {
    return static_cast<T>(luaL_optinteger(L, n, d));
  }

  int push_error_string(lua_State* L, PRL_RESULT result);
  int push_error(lua_State* L, PRL_RESULT result);
}

#endif
