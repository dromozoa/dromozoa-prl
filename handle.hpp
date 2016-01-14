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

#ifndef DROMOZOA_HANDLE_HPP
#define DROMOZOA_HANDLE_HPP

extern "C" {
#include <lua.h>
}

#include <PrlTypes.h>

namespace dromozoa {
  int new_handle(lua_State* L, PRL_HANDLE handle);
  PRL_HANDLE get_handle(lua_State* L, int n);
  PRL_RESULT free_handle(PRL_HANDLE handle);
  void initialize_handle_gc(lua_State* L);
  int open_handle(lua_State* L);
}

#endif
