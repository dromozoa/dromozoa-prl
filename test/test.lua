local prl = require "dromozoa.prl"

assert(not prl.sdk_wrap.is_loaded())
assert(prl.sdk_wrap.load_lib_from_std_paths())
assert(prl.sdk_wrap.is_loaded())

assert(prl.api.init_ex())

local server = assert(prl.api.server.create())
assert(server:get_type() == "PHT_SERVER")

local job = assert(server:login_local())
assert(job:get_type() == "PHT_JOB")
assert(job:wait())
assert(job:free())
assert(server:free())

assert(prl.api.deinit())

assert(prl.sdk_wrap.unload())
assert(not prl.sdk_wrap.is_loaded())
