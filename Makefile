# Compiler and flags
CXX      = clang++
CXXFLAGS = -Iinclude -MMD -MP -Wall -Wextra -std=c++23

# Source and object files
SRC = $(wildcard src/*.cc) main.cc
OBJ = $(patsubst %.cc, build/%.o, $(notdir $(SRC)))
DEP = $(OBJ:.o=.d)
ARGS =

# Output
OUT  = build/program
DUMP = build/dump.txt

# -----------------------------
# Default build (release)
# -----------------------------
all: release

# Release build
release: CXXFLAGS += -O2
release: $(OUT) $(DUMP)

# Debug build
debug: CXXFLAGS += -g -O0
debug: $(OUT)

$(DUMP): $(OUT)
	objdump -d $(OUT) > $@

# Link step
$(OUT): $(OBJ)
	$(CXX) -o $@ $^

# Compile step (put .o into build/)
build/%.o: src/%.cc | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: %.cc | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build/ exists
build:
	mkdir -p build

# -----------------------------
# Convenience targets
# -----------------------------
run: $(OUT)
	./$(OUT) $(ARGS)

clean:
	rm -rf build

-include $(DEP)

.PHONY: all clean build release debug run

