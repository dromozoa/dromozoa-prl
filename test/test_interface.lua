-- Copyright (C) 2015,2016 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local prl = require "dromozoa.prl"

assert(prl.sdk_wrap.load_lib_from_std_paths())
assert(prl.init_ex())

do
  local server = assert(prl.server.create())
  local job = assert(server:login_local())
  local a, b = assert(job:wait():get_ret_code())
  assert(prl.succeeded(b), a)
  local job = assert(server:get_vm_list())
  local a, b = assert(job:wait():get_ret_code())
  assert(prl.succeeded(b), a)
  local vm_list = assert(job:get_result())
  print(vm_list:get_params_count())
end
collectgarbage()
collectgarbage()

assert(prl.deinit())
assert(prl.sdk_wrap.unload())
