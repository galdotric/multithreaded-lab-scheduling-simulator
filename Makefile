CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

TARGET = main
SOURCES = main.cpp group.cpp student.cpp threads.cpp labs.cpp validation.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TEST_TARGET = test_validation
TEST_SOURCES = test_validation.cpp validation.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_OBJECTS) $(TEST_TARGET)

.PHONY: all clean test
