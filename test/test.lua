local sdk_wrap = require "dromozoa.prl.sdk_wrap"
local api = require "dromozoa.prl.api"

assert(not sdk_wrap.is_loaded())
-- assert(sdk_wrap.load_lib_from_std_paths(true))
assert(sdk_wrap.load("libprl_sdk.dylib", true))
assert(sdk_wrap.is_loaded())

-- assert(api.init_ex(api.API_VER, api.PAM_DESKTOP_MAC))
assert(api.deinit())

assert(sdk_wrap.unload())
assert(not sdk_wrap.is_loaded())
