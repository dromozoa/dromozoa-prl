local prl = require "dromozoa.prl"

assert(not prl.sdk_wrap.is_loaded())
assert(prl.sdk_wrap.load_lib_from_std_paths())
assert(prl.sdk_wrap.is_loaded())

assert(prl.api.init_ex())

local server = assert(prl.api.server.create())
assert(prl.api.handle.get_type(server) == "PHT_SERVER")

local job = assert(prl.api.server.login_local(server))
print(prl.api.handle.get_type(job))
assert(prl.api.job.wait(job))
assert(job:free())
-- assert(prl.api.handle.free(job))
assert(prl.api.handle.free(server))

-- print(prl.api.handle)
-- print(getmetatable(prl.api.job).__index)
-- print(prl.api.job.free)

assert(prl.api.deinit())

assert(prl.sdk_wrap.unload())
assert(not prl.sdk_wrap.is_loaded())
