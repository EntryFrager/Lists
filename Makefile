CC = g++

CFLAGS = -c -I -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline					\
	-Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default				\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy            \
	-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing   \
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

INCLUDES = include/list.h error/error.h utilities/utils.cpp

SOURCES = main.cpp list.cpp error/error.cpp utilities/utils.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = list.exe

VPATH = src/

all: $(SOURCES) $(INCLUDES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o: $(INCLUDES) $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf *.o list
	rm -rf list.exe
	rm -rf list.exe.log
	rm -rf list.exe.log.dmp
	rm -rf ./debug/dot.html
	rm -rf ./debug/dot.svg
	rm -rf ./debug/dump.dot
	rm -rf ./debug/file_err.txt
