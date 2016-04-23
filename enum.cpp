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

#include "common.hpp"

namespace dromozoa {
  void initialize_enum(lua_State* L) {
    // PRL_API_COMMAND_FLAGS
    luaX_set_field<lua_Integer>(L, -1, "PACF_NORMAL_SECURITY", PACF_NORMAL_SECURITY);
    luaX_set_field<lua_Integer>(L, -1, "PACF_HIGH_SECURITY", PACF_HIGH_SECURITY);
    luaX_set_field<lua_Integer>(L, -1, "PACF_NON_INTERACTIVE_MODE", PACF_NON_INTERACTIVE_MODE);
    luaX_set_field<lua_Integer>(L, -1, "PACF_CANCEL_TASK_ON_END_SESSION", PACF_CANCEL_TASK_ON_END_SESSION);

    // PRL_API_INIT_FLAGS
    luaX_set_field<lua_Integer>(L, -1, "PAIF_USE_GRAPHIC_MODE", PAIF_USE_GRAPHIC_MODE);
    luaX_set_field<lua_Integer>(L, -1, "PAIF_INIT_AS_APPSTORE_CLIENT", PAIF_INIT_AS_APPSTORE_CLIENT);

    // PRL_APPLICATION_MODE
    luaX_set_field<lua_Integer>(L, -1, "PAM_UNKNOWN", PAM_UNKNOWN);
    luaX_set_field<lua_Integer>(L, -1, "PAM_SERVER", PAM_SERVER);
    luaX_set_field<lua_Integer>(L, -1, "PAM_DESKTOP_MAC", PAM_DESKTOP_MAC);
    luaX_set_field<lua_Integer>(L, -1, "PAM_WORKSTATION_EXTREME", PAM_WORKSTATION_EXTREME);
    luaX_set_field<lua_Integer>(L, -1, "PAM_PLAYER", PAM_PLAYER);
    luaX_set_field<lua_Integer>(L, -1, "PAM_DESKTOP_STM", PAM_DESKTOP_STM);
    luaX_set_field<lua_Integer>(L, -1, "PAM_DESKTOP_WL", PAM_DESKTOP_WL);
    luaX_set_field<lua_Integer>(L, -1, "PAM_MOBILE", PAM_MOBILE);
    luaX_set_field<lua_Integer>(L, -1, "PAM_DESKTOP_STM_OBSOLETE", PAM_DESKTOP_STM_OBSOLETE);
    luaX_set_field<lua_Integer>(L, -1, "PAM_DESKTOP", PAM_DESKTOP);
    luaX_set_field<lua_Integer>(L, -1, "PAM_WORKSTATION", PAM_WORKSTATION);
    luaX_set_field<lua_Integer>(L, -1, "PAM_STM", PAM_STM);

    // PRL_DISPLAY_CODEC_TYPE
    luaX_set_field<lua_Integer>(L, -1, "PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION", PDCT_HIGH_QUALITY_WITHOUT_COMPRESSION);
    luaX_set_field<lua_Integer>(L, -1, "PDCT_HIGH_QUALITY_WITH_COMPRESSION", PDCT_HIGH_QUALITY_WITH_COMPRESSION);
    luaX_set_field<lua_Integer>(L, -1, "PDCT_MEDIUM_QUALITY_WITHOUT_COMPRESSION", PDCT_MEDIUM_QUALITY_WITHOUT_COMPRESSION);
    luaX_set_field<lua_Integer>(L, -1, "PDCT_MEDIUM_QUALITY_WITH_COMPRESSION", PDCT_MEDIUM_QUALITY_WITH_COMPRESSION);
    luaX_set_field<lua_Integer>(L, -1, "PDCT_LOW_QUALITY_WITHOUT_COMPRESSION", PDCT_LOW_QUALITY_WITHOUT_COMPRESSION);
    luaX_set_field<lua_Integer>(L, -1, "PDCT_LOW_QUALITY_WITH_COMPRESSION", PDCT_LOW_QUALITY_WITH_COMPRESSION);

    // PRL_KEY_EVENT
    luaX_set_field<lua_Integer>(L, -1, "PKE_PRESS", PKE_PRESS);
    luaX_set_field<lua_Integer>(L, -1, "PKE_RELEASE", PKE_RELEASE);
  }
}
