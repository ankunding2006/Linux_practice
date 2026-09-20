CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic
LDLIBS := -pthread


TARGET := condition_varable
SOURCE := condition_varable.cpp

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)