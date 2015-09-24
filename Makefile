# $File: Makefile
# $Date: Sun Jun 12 22:56:54 2011 +0800

OBJ_DIR = obj
TARGET = xabell

PKGCONFIG_LIBS = alsa x11 sndfile
INCLUDE_DIR = -Isrc/include -Isrc
#DEFINES = -D_DEBUG_BUILD_
CXXFLAGS = -Wall -Wextra  -pthread \
		   $(shell pkg-config --cflags $(PKGCONFIG_LIBS)) $(INCLUDE_DIR) $(DEFINES) -O2
LDFLAGS = -pthread \
		  $(shell pkg-config --libs $(PKGCONFIG_LIBS))

CXX = g++
CXXSOURCES = $(shell find src -name "*.cpp")
OBJS = $(addprefix $(OBJ_DIR)/,$(CXXSOURCES:.cpp=.o))
DEPFILES = $(OBJS:.o=.d)

all:  $(TARGET)


.PHONY: all clean run hg


$(OBJ_DIR)/%.o: %.cpp
	@echo "[cxx] $< ..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.d: %.cpp
	@mkdir -pv $(dir $@)
	@echo "[dep] $< ..."
	@$(CXX) $(INCLUDE_DIR) $(DEFINES) -MM -MT "$(OBJ_DIR)/$(<:.cpp=.o) $(OBJ_DIR)/$(<:.cpp=.d)" "$<"  > "$@"

sinclude $(DEPFILES)

$(TARGET): $(OBJS)
	@echo "Linking ..."
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)


clean:
	rm -rf $(OBJ_DIR)/* $(TARGET)

run: $(TARGET)
	./$(TARGET) bell.wav 100

install:
	cp xabell /usr/local/bin

hg:
	hg addremove
	hg commit -u jiakai
	hg push

