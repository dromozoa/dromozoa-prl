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

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "dromozoa/bind.hpp"

#include "api.hpp"
#include "enum.hpp"
#include "handle.hpp"
#include "result.hpp"
#include "job.hpp"
#include "error.hpp"
#include "server.hpp"
#include "virtual_machine.hpp"
#include "vm_configuration.hpp"
#include "sdk_wrap.hpp"

namespace dromozoa {
  using bind::function;
  using bind::get_log_level;
  using bind::push_success;
  using bind::set_field;

  int open_key(lua_State* L);

  inline void initialize_core(lua_State* L) {
    open_handle(L);
    lua_setfield(L, -2, "handle");

    open_job(L);
    lua_setfield(L, -2, "job");

    open_result(L);
    lua_setfield(L, -2, "result");

    open_server(L);
    lua_setfield(L, -2, "server");

    open_vm_configuration(L);
    lua_setfield(L, -2, "vm_configuration");

    open_virtual_machine(L);
    lua_setfield(L, -2, "virtual_machine");

    open_key(L);
    lua_setfield(L, -2, "key");
  }

  int open(lua_State* L) {
    lua_newtable(L);

    open_sdk_wrap(L);
    lua_setfield(L, -2, "sdk_wrap");

    bind::initialize(L);
    initialize_api(L);
    initialize_core(L);
    initialize_enum(L);

    return 1;
  }
}

extern "C" int luaopen_dromozoa_prl(lua_State* L) {
  return dromozoa::open(L);
}
