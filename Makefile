FONTS = ./fonts
SOURCE = ./source

OBJ_CPP = $(wildcard ${SOURCE}/*.cpp) \
		  $(wildcard ${SOURCE}/drawers/buffer/*.cpp) \
		  $(wildcard ${SOURCE}/drawers/gl/*.cpp) \
		  $(wildcard ${SOURCE}/net/*.cpp) \
          $(wildcard ${SOURCE}/wrappers/*.cpp) \
		  $(wildcard ${SOURCE}/util/*.cpp)

TARGET = nametag

DEBUG_FLAGS+=-Og -ggdb -Wall -Wextra -Wformat -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-qual  -Wno-unused-parameter -Wfatal-errors -DDEBUGGING -Wsuggest-final-types -Wpedantic -Wnull-dereference -fno-omit-frame-pointer -fdiagnostics-color -ftemplate-depth=64 -fconstexpr-depth=64 -ftemplate-backtrace-limit=8 -Wreorder -Wold-style-cast -Woverloaded-virtual
OPT_FLAGS+=-O3 -march=native -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
DEV_FLAGS+=-DDEV_MODE

INCLUDE = -isystem /opt/vc/include -isystem include -I/usr/include/freetype2 -isystem include/uWebSockets/src -iquote source -isystem include/uWebSockets/uSockets/src

EMBEDDED_LIB = -lbcm2835 -lbrcmEGL -lbrcmGLESv2 -L/opt/vc/lib -lfreetype -lr -lz -Linclude/uWebSockets/uSockets/ -l:uSockets.a
DESKTOP_LIB = -lfreetype -lrt -lSDL2 -lGLESv2 -lz -Linclude/uWebSockets/uSockets/ -l:uSockets.a

CXXFLAGS+=-std=c++2a -pipe -Winvalid-pch $(INCLUDE) -fexceptions -Wno-psabi -pthread

all: default
default: debug

release: CXXFLAGS+=$(OPT_FLAGS)
release: LIBRARIES=$(EMBEDDED_LIB)
release: $(TARGET)

debug: CXXFLAGS+=$(DEBUG_FLAGS)
debug: LIBRARIES=$(EMBEDDED_LIB)
debug: $(TARGET)

devmode: CXXFLAGS+=$(DEBUG_FLAGS) $(DEV_FLAGS)
devmode: LIBRARIES=$(DESKTOP_LIB)
devmode: $(TARGET)

OBJ_O = $(OBJ_CPP:.cpp=.o)

$(TARGET): ${OBJ_O}
	-@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) $(OBJ_O) -o $@ $(LIBRARIES)

%.o : %.cpp
	-@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) -c  $< -o $@ $(LIBRARIES)

clean:
	find $(OBJECTS) -name "*.o" -type f -delete
