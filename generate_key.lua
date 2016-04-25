-- Copyright (C) 2015 Tomoyuki Fujimori <moyu@dromozoa.com>
--
-- This file is part of dromozoa-prl.
--
-- dromozoa-prl is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- dromozoa-prl is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

io.write([[
#include "common.hpp"

namespace dromozoa {
  void initialize_key(lua_State* L) {
    lua_newtable(L);
    {
]])

for line in io.lines() do
  local key = line:match("PRL_KEY_(%S+)%s*=%s*%d+")
  if key ~= nil and key ~= "INVALID" and key ~= "MAX" then
    io.write("      luaX_set_field<lua_Integer>(L, -1, \"", key, "\", PRL_KEY_", key, ");\n")
  end
end

io.write([[
    }
    luaX_set_field(L, -2, "key");
  }
}
]])
