extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <string>
#include <iostream>

namespace {
  int impl_load(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    PRL_RESULT result = SdkWrap_Load(name);
    if (PRL_FAILED(result)) {
      lua_pushnil(L);
      lua_pushstring(L, "could not SdkWrap_Load");
      return 2;
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  }

  int impl_unload(lua_State* L) {
    PRL_RESULT result = SdkWrap_Unload();
    if (PRL_FAILED(result)) {
      lua_pushnil(L);
      lua_pushstring(L, "could not SdkWrap_Unload");
      return 2;
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  }
}

extern "C" int luaopen_dromozoa_prl_sdk_wrap(lua_State* L) {
  lua_newtable(L);
  lua_pushcfunction(L, impl_load);
  lua_setfield(L, -2, "load");
  lua_pushcfunction(L, impl_unload);
  lua_setfield(L, -2, "unload");
  return 1;
}
