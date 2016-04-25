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

#ifndef DROMOZOA_COMMON_HPP
#define DROMOZOA_COMMON_HPP

#include <SdkWrap.h>

#include <dromozoa/bind.hpp>

namespace dromozoa {
  class handle_reference {
  public:
    explicit handle_reference(PRL_HANDLE handle);
    ~handle_reference();
    PRL_RESULT free();
    PRL_HANDLE get() const;
    intptr_t get_address() const;
  private:
    PRL_HANDLE handle_;
    handle_reference(const handle_reference&);
    handle_reference& operator=(const handle_reference&);
  };

  std::string result_to_string(PRL_RESULT result);
  void push_error(lua_State* L, PRL_RESULT result);

  void new_handle(lua_State* L, PRL_HANDLE handle);
  handle_reference* check_handle_reference(lua_State* L, int arg);
  PRL_HANDLE check_handle(lua_State* L, int arg);
  void inherit_handle(lua_State* L, const char* name);
  void inherit_handle(lua_State* L, const char* name, const char* super);
}

#endif
