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

assert(prl.get_log_level() == 0)
prl.set_log_level(3)

assert(prl.sdk_wrap.load("libprl_sdk.dylib"))
assert(prl.init_ex())

do
  local server = assert(prl.server.create())
  server:login_local():wait():check_ret_code():free()

  local job = server:get_vm_list():wait():check_ret_code()
  -- local vm_list = job:get_result_and_free()
  local vm_list = job:get_result()
  -- job:free()
  print(vm_list:get_params_count())
  vm_list:free()
  server:free()
end
-- collectgarbage()
-- collectgarbage()

assert(prl.deinit())
assert(prl.sdk_wrap.unload())
