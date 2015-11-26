PRL_SDKDIR = /Library/Frameworks/ParallelsVirtualizationSDK.framework

CPPFLAGS = -DDYN_API_WRAP -I$(LUA_INCDIR) -I$(PRL_SDKDIR)/Headers -I$(PRL_SDKDIR)/Libraries/Helpers/SdkWrap
CXXFLAGS = $(CFLAGS)
LDFLAGS = -L$(LUA_LIBDIR) $(LIBFLAG)
LDLIBS = -ldl

TARGET = sdk.so

all: $(TARGET)

clean:
	rm -f *.o $(TARGET)

sdk.so: sdk.o SdkWrap.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

sdk.o: sdk.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

SdkWrap.o: $(PRL_SDKDIR)/Libraries/Helpers/SdkWrap/SdkWrap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

install:
	mkdir -p "$(LIBDIR)/dromozoa/prl"
	cp sdk.so "$(LIBDIR)/dromozoa/prl"
