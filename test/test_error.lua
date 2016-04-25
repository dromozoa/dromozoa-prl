-- Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local a, b, c = prl.sdk_wrap.unload()
assert(a == nil)
assert(b == "PRL_RESULT_DECLARE_ERROR(80000007)")
assert(c == 0x0007 - 0x80000000)
assert(prl.sdk_wrap.load_lib_from_std_paths())

local a, b, c = prl.deinit()
assert(a == nil)
assert(b == "PRL_ERR_API_WASNT_INITIALIZED")
assert(c == 0x0287 - 0x80000000)

assert(prl.init_ex())
assert(prl.deinit())

local a, b, c = prl.deinit()
assert(a == nil)
assert(b == "PRL_ERR_API_WASNT_INITIALIZED")
assert(c == 0x0287 - 0x80000000)

assert(prl.sdk_wrap.unload())
