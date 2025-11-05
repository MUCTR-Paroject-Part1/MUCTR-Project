CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -Isrc
TARGET = build/main.exe
TEST_TARGET = build/tests.exe

# Main application sources
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
SOURCES = $(SRC_DIR)/main.cpp \
          $(CORE_DIR)/BurgerInventory.cpp \
          $(CORE_DIR)/TableManager.cpp \
          $(CORE_DIR)/Manager.cpp \
          $(CORE_DIR)/Customer.cpp \
          $(CORE_DIR)/Restaurant.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=build/%.o)

# Test sources
TEST_DIR = tests
TEST_SOURCES = $(TEST_DIR)/test_main.cpp \
               $(TEST_DIR)/test_BurgerInventory.cpp \
               $(TEST_DIR)/test_TableManager.cpp \
               $(TEST_DIR)/test_Manager.cpp \
               $(TEST_DIR)/test_Customer.cpp \
               $(TEST_DIR)/test_Restaurant.cpp
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=build/%.o)

# Common sources for tests
COMMON_SOURCES = $(CORE_DIR)/BurgerInventory.cpp \
                 $(CORE_DIR)/TableManager.cpp \
                 $(CORE_DIR)/Manager.cpp \
                 $(CORE_DIR)/Customer.cpp \
                 $(CORE_DIR)/Restaurant.cpp
COMMON_OBJECTS = $(COMMON_SOURCES:$(CORE_DIR)/%.cpp=build/core/%.o)

# Create build directories
build:
	@mkdir -p build/core
	@mkdir -p build/tests

compile: build $(TARGET)

$(TARGET): $(OBJECTS) | build
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

build/%.o: $(SRC_DIR)/%.cpp | build
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Test compilation
test: build $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(COMMON_OBJECTS) | build
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS) $(COMMON_OBJECTS)

build/%.o: $(TEST_DIR)/%.cpp | build
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -Itests -c $< -o $@

run-tests: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf build

.PHONY: compile run clean test run-tests build
