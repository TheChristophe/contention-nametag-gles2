FONTS = ./fonts
SOURCE = ./source
OBJECTS = ./obj

OBJ_CPP = $(wildcard ${SOURCE}/*.cpp) \
		  $(wildcard ${SOURCE}/drawers/buffer/*.cpp) \
		  $(wildcard ${SOURCE}/drawers/gl/*.cpp) \
          $(wildcard ${SOURCE}/wrappers/*.cpp) \
		  $(wildcard ${SOURCE}/util/*.cpp)
OBJ_O = $(patsubst %.cpp,${OBJECTS}/%.o,$(subst ./source/,,${OBJ_CPP}))

TARGET = nametag

DEBUG_FLAGS+=-Og -ggdb -Wall -Wextra -Wformat -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-qual  -Wno-unused-parameter -Wfatal-errors -DDEBUGGING -Wsuggest-final-types -Wpedantic -Wnull-dereference -fno-omit-frame-pointer -fdiagnostics-color -ftemplate-depth=64 -fconstexpr-depth=64 -ftemplate-backtrace-limit=8 -Wreorder -Wold-style-cast -Woverloaded-virtual
OPT_FLAGS+=-O3 -march=native -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard

INCLUDE = -isystem /opt/vc/include -isystem include -I/usr/include/freetype2 -iquote source
LIB = -lbcm2835 -lbrcmEGL -lbrcmGLESv2 -L/opt/vc/lib -lfreetype

CXXFLAGS+=-std=c++17 -pipe -Winvalid-pch $(INCLUDE) -fexceptions -Wno-psabi

all: default
default: debug

release: CXXFLAGS+=$(OPT_FLAGS)
release: $(TARGET)

debug: CXXFLAGS+=$(DEBUG_FLAGS)
debug: $(TARGET)

$(TARGET): ${OBJ_O}
	-@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) $(OBJ_O) -o $@ $(LIB)

${OBJECTS}/%.o : $(SOURCE)/%.cpp
	-@echo "Compiling $@..."
	@$(CXX) $(CXXFLAGS) -c  $< -o $@ $(LIB)
	
clean:
	find $(OBJECTS) -name "*.o" -type f -delete
