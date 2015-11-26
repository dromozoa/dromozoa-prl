PRL_SDKDIR = /Library/Frameworks/ParallelsVirtualizationSDK.framework
PRL_SDKWRAPDIR = $(PRL_SDKDIR)/Libraries/Helpers/SdkWrap

CPPFLAGS = -DDYN_API_WRAP -I$(LUA_INCDIR) -I$(PRL_SDKDIR)/Headers -I$(PRL_SDKWRAPDIR)
CXXFLAGS = $(CFLAGS)
LDFLAGS = -L$(LUA_LIBDIR) $(LIBFLAG)
LDLIBS = -ldl

TARGET = sdk_wrap.so

all: $(TARGET)

clean:
	rm -f *.o $(TARGET)

sdk_wrap.so: sdk_wrap.o SdkWrap.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

sdk_wrap.o: sdk_wrap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

SdkWrap.o: $(PRL_SDKWRAPDIR)/SdkWrap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

install:
	mkdir -p "$(LIBDIR)/dromozoa/prl"
	cp sdk_wrap.so "$(LIBDIR)/dromozoa/prl"
