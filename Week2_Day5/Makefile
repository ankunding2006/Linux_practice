CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic
LDLIBS := -pthread


TARGET := ThreadSafeQueue
SOURCE := ThreadSafeQueue.cpp

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)