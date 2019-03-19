CC := g++
CFLAGS := -g -Wall -std=c++14
LIBS := -lallegro -lallegro_main -lallegro_primitives -I/usr/local/include/allegro5 # macOS
#LIBS := -lallegro -I/usr/local/include/allegro5 #linux
SOURCES := $(shell find src/ -type f -name "*.cpp")
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := game

all: $(SOURCES) $(TARGET)
	./$(TARGET)

run: all

$(TARGET): $(OBJECTS)
	@echo " Linking..."; $(CC) $^ -o $@ $(LIBS)

%.o: %.cpp
	@echo " CC $<"; $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning..."; $(RM) src/*.o $(TARGET)

.PHONY: all clean
