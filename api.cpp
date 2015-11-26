extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include <cstdint>
#include <iostream>

namespace dromozoa {
  inline void set_field(lua_State* L, const char* key, lua_Integer value) {
    lua_pushinteger(L, value);
    lua_setfield(L, -2, key);
  }

  inline void set_field(lua_State* L, const char* key, const char* value) {
    lua_pushstring(L, value);
    lua_setfield(L, -2, key);
  }

  inline void set_field(lua_State* L, const char* key, lua_CFunction value) {
    lua_pushcfunction(L, value);
    lua_setfield(L, -2, key);
  }

  inline int return_error(lua_State* L, int result) {
    std::cerr << "return_error:" << result << std::endl;
    lua_pushnil(L);
    // lua_pushstring(L, prl_result_to_string(result));
    lua_pushstring(L, "error");
    return 2;
  }
}

#define DROMOZOA_SET_FIELD(value) \
  dromozoa::set_field(L, #value, (value))

extern "C" int luaopen_dromozoa_prl_api(lua_State* L) {
  lua_newtable(L);

  dromozoa::set_field(L, "API_VER", PARALLELS_API_VER);
  dromozoa::set_field(L, "SDK_LIB_MAJ", PARALLELS_SDK_LIB_MAJ);

  DROMOZOA_SET_FIELD(PAM_UNKNOWN);
  DROMOZOA_SET_FIELD(PAM_SERVER);
  DROMOZOA_SET_FIELD(PAM_DESKTOP_MAC);
  DROMOZOA_SET_FIELD(PAM_WORKSTATION_EXTREME);
  DROMOZOA_SET_FIELD(PAM_PLAYER);
  DROMOZOA_SET_FIELD(PAM_DESKTOP_STM);
  DROMOZOA_SET_FIELD(PAM_DESKTOP_WL);
  DROMOZOA_SET_FIELD(PAM_MOBILE);

  dromozoa::set_field(L, "init_ex", [](lua_State* L){
    std::cerr << "init_ex" << std::endl;
    lua_Integer version = luaL_checkinteger(L, 1);
    lua_Integer mode = luaL_checkinteger(L, 2);
    PRL_RESULT result = PrlApi_InitEx(version, static_cast<PRL_APPLICATION_MODE>(mode), 0, 0);
    if (PRL_FAILED(result)) {
      return dromozoa::return_error(L, result);
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  });

  dromozoa::set_field(L, "deinit", [](lua_State* L){
    std::cerr << "deinit " << PrlSdkWrapNamespace::PrlApi_Deinit << std::endl;
    PRL_RESULT result = PrlSdkWrapNamespace::PrlApi_Deinit();
    if (PRL_FAILED(result)) {
      return dromozoa::return_error(L, result);
    } else {
      lua_pushboolean(L, true);
      return 1;
    }
  });

  return 1;
}
