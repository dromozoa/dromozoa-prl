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

local prl = require "dromozoa.prl"

assert(not prl.sdk_wrap.is_loaded())
assert(prl.sdk_wrap.load_lib_from_std_paths())
assert(prl.sdk_wrap.is_loaded())

assert(prl.init_ex())

local server = assert(prl.server.create())
assert(server:get_type() == "PHT_SERVER")

local job = assert(server:login_local())
assert(job:get_type() == "PHT_JOB")
assert(job:wait())
assert(job:get_ret_code() == "PRL_ERR_SUCCESS")
assert(job:free())

local job = assert(server:get_vm_list())
assert(job:wait())
assert(job:get_ret_code() == "PRL_ERR_SUCCESS")

local vm_list = assert(job:get_result())
assert(vm_list:get_type() == "PHT_RESULT")
for i = 0, vm_list:get_params_count() - 1 do
  local vm = vm_list:get_param_by_index(i)
  assert(vm:get_type() == "PHT_VIRTUAL_MACHINE")
  local vm_config = vm:get_config()
  assert(vm_config:get_type() == "PHT_VIRTUAL_MACHINE")
  assert(vm:get_address() == vm_config:get_address())
  print(string.format("%q", vm_config:get_name()))
  assert(vm_config:free())
  assert(vm:free())
end
assert(vm_list:free())

assert(job:free())

local job = assert(server:logoff())
assert(job:wait())
assert(job:get_ret_code() == "PRL_ERR_SUCCESS")
assert(job:free())

assert(server:free())

assert(prl.deinit())
assert(prl.sdk_wrap.unload())
assert(not prl.sdk_wrap.is_loaded())
