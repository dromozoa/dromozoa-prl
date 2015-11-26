local prl = require "dromozoa.prl"

assert(not prl.sdk_wrap.is_loaded())
-- assert(sdk_wrap.load_lib_from_std_paths(true))
assert(prl.sdk_wrap.load("libprl_sdk.dylib"))
assert(prl.sdk_wrap.is_loaded())

-- assert(api.init_ex(api.API_VER, api.PAM_DESKTOP_MAC))
assert(prl.api.deinit())

assert(prl.sdk_wrap.unload())
assert(not prl.sdk_wrap.is_loaded())
