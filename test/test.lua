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

prl.set_log_level(3)
prl.set_raise_error(true)

assert(not prl.sdk_wrap.is_loaded())
prl.sdk_wrap.load_lib_from_std_paths()
assert(prl.sdk_wrap.is_loaded())
prl.init_ex()

local server = prl.server.create()
assert(server:get_type() == "PHT_SERVER")

local job = server:login_local()
assert(job:get_type() == "PHT_JOB")
job:wait()
assert(job:get_ret_code() == "PRL_ERR_SUCCESS")
job:free()

local job = server:get_vm_list()
job:wait()
assert(job:get_ret_code() == "PRL_ERR_SUCCESS")

local vm_list = job:get_result()
assert(vm_list:get_type() == "PHT_RESULT")
for i = 1, vm_list:get_params_count() do
  local vm = vm_list:get_param_by_index(i)
  assert(vm:get_type() == "PHT_VIRTUAL_MACHINE")
  local vm_config = vm:get_config()
  assert(vm_config:get_type() == "PHT_VIRTUAL_MACHINE")
  assert(vm:get_address() == vm_config:get_address())
  print(string.format("%q", vm_config:get_name()))
  vm_config:free()
  vm:free()
end
vm_list:free()
job:free()

local job = server:logoff()
job:wait()
assert(job:get_ret_code() == "PRL_ERR_SUCCESS")
job:free()

assert(server:free())

prl.deinit()
prl.sdk_wrap.unload()
