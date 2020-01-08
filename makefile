CC = g++
CFLAGS = -Wall -Iincludes
TARGET = HW6.exe
SRCFILES = $(wildcard src/*.cpp)
OBJFILES = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
DEPS = $(OBJFILES:.o=.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $^

-include $(DEPS)

%.d: %.cpp
	$(CC) -MM $(CFLAGS) $*.cpp > $@

%.o: %.cpp
	$(CC) $(CFLAGS) -g -c $*.cpp -o $@

.PHONY: clean
clean:
	echo Hello
	rm -f $(DEPS)
	rm -f $(OBJFILES)

.PHONY: rebuild
rebuild:
	rm -f $(TARGET)
	$(MAKE) clean
	$(MAKE) all
