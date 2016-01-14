# Copyright (C) 2015 Tomoyuki Fujimori <moyu@dromozoa.com>
#
# This file is part of dromozoa-prl.
#
# dromozoa-prl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# dromozoa-prl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

PRL_SDKDIR = /Library/Frameworks/ParallelsVirtualizationSDK.framework
PRL_SDKWRAPDIR = $(PRL_SDKDIR)/Libraries/Helpers/SdkWrap

CPPFLAGS = -DDYN_API_WRAP -Ibind -I$(LUA_INCDIR) -I$(PRL_SDKDIR)/Headers -I$(PRL_SDKWRAPDIR)
CXXFLAGS = -Wall -W -std=c++11 $(CFLAGS)
LDFLAGS = -L$(LUA_LIBDIR) $(LIBFLAG)
LDLIBS = -ldl

TARGET = prl.so
OBJS = \
	bind.o \
	error.o \
	handle.o \
	handle_job.o \
	handle_result.o \
	handle_server.o \
	handle_virtual_machine.o \
	handle_vm_configuration.o \
	result.o \
	sdk_wrap.o \
	module.o \
	SdkWrap.o

all: $(TARGET)

clean:
	rm -f key.hpp *.o $(TARGET)

prl.so: $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

key.hpp: $(PRL_SDKDIR)/Headers/PrlKeys.h
	$(LUA) generate_key.lua <$< >$@

bind.o: bind/bind.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

module.o: module.cpp key.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

SdkWrap.o: $(PRL_SDKWRAPDIR)/SdkWrap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

install:
	cp dromozoa-prl-create-centos7 $(BINDIR)
	cp dromozoa-prl-send-key $(BINDIR)
	cp dromozoa-prl-wait-centos7 $(BINDIR)
	mkdir -p $(LIBDIR)/dromozoa
	cp $(TARGET) $(LIBDIR)/dromozoa
