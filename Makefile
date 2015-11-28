PRL_SDKDIR = /Library/Frameworks/ParallelsVirtualizationSDK.framework
PRL_SDKWRAPDIR = $(PRL_SDKDIR)/Libraries/Helpers/SdkWrap

CPPFLAGS = -DDYN_API_WRAP -I$(LUA_INCDIR) -I$(PRL_SDKDIR)/Headers -I$(PRL_SDKWRAPDIR)
CXXFLAGS = -Wall -W -std=c++11 $(CFLAGS)
LDFLAGS = -L$(LUA_LIBDIR) $(LIBFLAG)
LDLIBS = -ldl

TARGET = prl.so

all: $(TARGET)

clean:
	rm -f key.hpp *.o $(TARGET)

prl.so: prl.o SdkWrap.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

key.hpp: $(PRL_SDKDIR)/Headers/PrlKeys.h
	$(LUA) generate_key.lua <$< >$@

prl.o: prl.cpp key.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

SdkWrap.o: $(PRL_SDKWRAPDIR)/SdkWrap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

install:
	cp dromozoa-prl-send-key $(BINDIR)
	mkdir -p $(LIBDIR)/dromozoa
	cp $(TARGET) $(LIBDIR)/dromozoa
