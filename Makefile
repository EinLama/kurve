CC := g++
CFLAGS := -g -Wall
LIBS := -lallegro -lallegro_main -lallegro_primitives -I/usr/local/include/allegro5 # macOS
#LIBS := -lallegro -I/usr/local/include/allegro5 #linux
SOURCES := $(shell find src/ -type f -name "*.cc")
OBJECTS := $(SOURCES:.cc=.o)
TARGET := game

all: $(SOURCES) $(TARGET)
	./$(TARGET)

run: all

$(TARGET): $(OBJECTS)
	@echo " Linking..."; $(CC) $^ -o $@ $(LIBS)

%.o: %.c
	@echo " CC $<"; $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning..."; $(RM) src/*.o $(TARGET)

.PHONY: all clean
