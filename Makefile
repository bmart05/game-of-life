CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra $(shell pkg-config --cflags sdl3)
LDLIBS   := $(shell pkg-config --libs sdl3)

TARGET   := conway
SRCS     := $(wildcard *.cpp)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)
	rm -f *.o *.d

run: $(TARGET)
	./$(TARGET) "$(FILE)"

.PHONY: clean run
clean:
	rm -f *.o *.d $(TARGET)