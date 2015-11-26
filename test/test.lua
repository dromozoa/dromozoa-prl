local prl = require "dromozoa.prl"

assert(not prl.sdk_wrap.is_loaded())
assert(prl.sdk_wrap.load_lib_from_std_paths())
assert(prl.sdk_wrap.is_loaded())

assert(prl.api.init_ex())

local server = assert(prl.api.server.create())
assert(server:get_type() == "PHT_SERVER")

print("--", server:get_type())
print(server)
print(getmetatable(server))
print(getmetatable(server).__index)
print(getmetatable(server).name)
print(server.get_vm_list)
print(server.login_local)
print(server.logoff)

local job = assert(server:login_local())

print("--", server:get_type())
print(server)
print(getmetatable(server))
print(getmetatable(server).__index)
print(getmetatable(server).name)
print(server.get_vm_list)
print(server.login_local)
print(server.logoff)

print("--", job:get_type())
print(job)
print(getmetatable(job))
print(getmetatable(job).__index)
print(getmetatable(job).name)
print(job.get_vm_list)
print(job.login_local)
print(job.logoff)



assert(job:get_type() == "PHT_JOB")
assert(job:wait())
assert(job:free())

local job = assert(server:logoff())
assert(job:wait())
assert(job:free())

assert(server:free())

assert(prl.api.deinit())

assert(prl.sdk_wrap.unload())
assert(not prl.sdk_wrap.is_loaded())
