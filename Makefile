PRL_SDKDIR = /Library/Frameworks/ParallelsVirtualizationSDK.framework
PRL_SDKWRAPDIR = $(PRL_SDKDIR)/Libraries/Helpers/SdkWrap

CPPFLAGS = -DDYN_API_WRAP -I$(LUA_INCDIR) -I$(PRL_SDKDIR)/Headers -I$(PRL_SDKWRAPDIR)
CXXFLAGS = -std=c++11 $(CFLAGS)
LDFLAGS = -L$(LUA_LIBDIR) $(LIBFLAG)
LDLIBS = -ldl

TARGET = prl.so

all: $(TARGET)

clean:
	rm -f prl_key.hpp *.o $(TARGET)

prl.so: prl.o SdkWrap.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

prl_key.hpp: $(PRL_SDKDIR)/Headers/PrlKeys.h
	$(LUA) generate_key.lua <$< >$@

prl.o: prl_key.hpp prl.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c prl.cpp

SdkWrap.o: $(PRL_SDKWRAPDIR)/SdkWrap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

install:
	mkdir -p "$(LIBDIR)/dromozoa"
	cp $(TARGET) "$(LIBDIR)/dromozoa"
