CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
TARGET = main.exe
SOURCES = main.cpp BurgerInventory.cpp TableManager.cpp Manager.cpp Customer.cpp Restaurant.cpp
OBJECTS = $(SOURCES:.cpp=.o)

compile: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: compile run clean