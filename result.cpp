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
}

#include <SdkWrap.h>

#include "dromozoa/bind.hpp"

#include "error.hpp"
#include "handle.hpp"
#include "result.hpp"

namespace dromozoa {
  using bind::push_success;

  int push_result(lua_State* L, PRL_RESULT result) {
    if (PRL_SUCCEEDED(result)) {
      return push_success(L);
    } else {
      return push_error(L, result);
    }
  }

  int push_result(lua_State* L, PRL_RESULT result, PRL_RESULT code) {
    if (PRL_SUCCEEDED(result)) {
      // [TODO] 仕様決定
      push_error_string(L, code);
      lua_pushinteger(L, code);
      return 2;
    } else {
      return push_error(L, result);
    }
  }

  int push_result(lua_State* L, PRL_RESULT result, PRL_HANDLE handle) {
    if (PRL_SUCCEEDED(result)) {
      new_handle(L, handle);
      return 1;
    } else {
      return push_error(L, result);
    }
  }

  int push_result(lua_State* L, PRL_HANDLE handle) {
    if (handle != PRL_INVALID_HANDLE) {
      new_handle(L, handle);
      return 1;
    } else {
      return push_error(L, PRL_ERR_INVALID_HANDLE);
    }
  }
}
