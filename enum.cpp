// Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-prl.
//
// dromozoa-prl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-prl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <SdkWrap.h>

#include "dromozoa/bind.hpp"

namespace dromozoa {
  namespace detail {
    lua_Integer check_enum(lua_State* L, int n) {
      return luaL_checkinteger(L, n);
    }

    lua_Integer opt_enum(lua_State* L, int n, lua_Integer d) {
      return luaL_optinteger(L, n, d);
    }
  }

  void initialize_enum(lua_State* L) {
    // PRL_API_COMMAND_FLAGS
    DROMOZOA_BIND_SET_FIELD(L, PACF_NORMAL_SECURITY);
    DROMOZOA_BIND_SET_FIELD(L, PACF_HIGH_SECURITY);
    DROMOZOA_BIND_SET_FIELD(L, PACF_NON_INTERACTIVE_MODE);
    DROMOZOA_BIND_SET_FIELD(L, PACF_CANCEL_TASK_ON_END_SESSION);

    // PRL_API_INIT_FLAGS
    DROMOZOA_BIND_SET_FIELD(L, PAIF_USE_GRAPHIC_MODE);
    DROMOZOA_BIND_SET_FIELD(L, PAIF_INIT_AS_APPSTORE_CLIENT);

    // PRL_APPLICATION_MODE
    DROMOZOA_BIND_SET_FIELD(L, PAM_UNKNOWN);
    DROMOZOA_BIND_SET_FIELD(L, PAM_SERVER);
    DROMOZOA_BIND_SET_FIELD(L, PAM_DESKTOP_MAC);
    DROMOZOA_BIND_SET_FIELD(L, PAM_WORKSTATION_EXTREME);
    DROMOZOA_BIND_SET_FIELD(L, PAM_PLAYER);
    DROMOZOA_BIND_SET_FIELD(L, PAM_DESKTOP_STM);
    DROMOZOA_BIND_SET_FIELD(L, PAM_DESKTOP_WL);
    DROMOZOA_BIND_SET_FIELD(L, PAM_MOBILE);
    DROMOZOA_BIND_SET_FIELD(L, PAM_DESKTOP_STM_OBSOLETE);
    DROMOZOA_BIND_SET_FIELD(L, PAM_DESKTOP);
    DROMOZOA_BIND_SET_FIELD(L, PAM_WORKSTATION);
    DROMOZOA_BIND_SET_FIELD(L, PAM_STM);

    // PRL_DISPLAY_CODEC_TYPE
    DROMOZOA_BIND_SET_FIELD(L, PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION);
    DROMOZOA_BIND_SET_FIELD(L, PDCT_HIGH_QUALITY_WITH_COMPRESSION);
    DROMOZOA_BIND_SET_FIELD(L, PDCT_MEDIUM_QUALITY_WITHOUT_COMPRESSION);
    DROMOZOA_BIND_SET_FIELD(L, PDCT_MEDIUM_QUALITY_WITH_COMPRESSION);
    DROMOZOA_BIND_SET_FIELD(L, PDCT_LOW_QUALITY_WITHOUT_COMPRESSION);
    DROMOZOA_BIND_SET_FIELD(L, PDCT_LOW_QUALITY_WITH_COMPRESSION);

    // PRL_KEY_EVENT
    DROMOZOA_BIND_SET_FIELD(L, PKE_PRESS);
    DROMOZOA_BIND_SET_FIELD(L, PKE_RELEASE);
  }
}
