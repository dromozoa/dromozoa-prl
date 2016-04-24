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

#include <vector>

#include "common.hpp"

namespace dromozoa {
  namespace {
    void impl_get_name(lua_State* L) {
      PRL_HANDLE handle = check_handle(L, 1);
      PRL_UINT32 size = 0;
      PRL_RESULT result = PrlVmCfg_GetName(handle, 0, &size);
      if (PRL_FAILED(result)) {
        push_error(L, result);
      } else {
        std::vector<char> buffer(size);
        result = PrlVmCfg_GetName(handle, &buffer[0], &size);
        if (PRL_FAILED(result)) {
          push_error(L, result);
        } else {
          lua_pushstring(L, &buffer[0]);
        }
      }
    }
  }

  void initialize_vm_configuration(lua_State* L) {
    lua_newtable(L);
    {
      luaL_getmetatable(L, "dromozoa.prl.handle");
      luaX_get_field(L, -1, "__index");
      luaX_set_metafield(L, -3, "__index");
      luaL_newmetatable(L, "dromozoa.prl.vm_configuration");
      lua_pushvalue(L, -3);
      luaX_set_field(L, -2, "__index");
      luaX_get_field(L, -2, "__gc");
      luaX_set_field(L, -2, "__gc");
      lua_pop(L, 2);

      luaX_set_field(L, -1, "get_name", impl_get_name);
    }
    luaX_set_field(L, -2, "vm_configuration");
  }
}
