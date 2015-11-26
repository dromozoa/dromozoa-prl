extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

namespace dromozoa {
  inline void set_field(lua_State* L, const char* key, lua_CFunction value) {
    lua_pushcfunction(L, value);
    lua_setfield(L, -2, key);
  }

  inline int return_error(lua_State* L, const char* message) {
    lua_pushnil(L);
    lua_pushstring(L, message);
    return 2;
  }
}

extern "C" int luaopen_dromozoa_prl_sdk_wrap(lua_State* L) {
  lua_newtable(L);

  dromozoa::set_field(L, "load", [](lua_State* L){
    const char* name = luaL_checkstring(L, 1);
    bool debug = lua_toboolean(L, 2);
    PRL_RESULT result = SdkWrap_Load(name, debug);
    if (PRL_FAILED(result)) {
      return dromozoa::return_error(L, "could not SdkWrap_Load");
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  });

  dromozoa::set_field(L, "unload", [](lua_State* L){
    PRL_RESULT result = SdkWrap_Unload();
    if (PRL_FAILED(result)) {
      return dromozoa::return_error(L, "could not SdkWrap_Unload");
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  });

  dromozoa::set_field(L, "is_loaded", [](lua_State* L){
    PRL_BOOL result = SdkWrap_IsLoaded();
    lua_pushboolean(L, result);
    return 1;
  });

  dromozoa::set_field(L, "load_lib_from_std_paths", [](lua_State* L) {
    bool debug = lua_toboolean(L, 1);
    PRL_BOOL result = SdkWrap_LoadLibFromStdPaths(debug);
    if (PRL_FAILED(result)) {
      return dromozoa::return_error(L, "could not SdkWrap_LoadLibFromStdPaths");
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  });

  return 1;
}
