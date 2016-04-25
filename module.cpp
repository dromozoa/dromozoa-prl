// Copyright (C) 2015,2016 Tomoyuki Fujimori <moyu@dromozoa.com>
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

#include "common.hpp"

namespace dromozoa {
  void initialize_api(lua_State* L);
  void initialize_enum(lua_State* L);
  void initialize_handle(lua_State* L);
  void initialize_key(lua_State* L);
  void initialize_sdk_wrap(lua_State* L);

  void initialize(lua_State* L) {
    initialize_api(L);
    initialize_enum(L);
    initialize_handle(L);
    initialize_key(L);
    initialize_sdk_wrap(L);
  }
}

extern "C" int luaopen_dromozoa_prl(lua_State* L) {
  lua_newtable(L);
  dromozoa::initialize(L);
  return 1;
}
