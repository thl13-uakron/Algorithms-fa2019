all:

%.o: %.cp
	g++ -c -O2 -Wall -Wextra -pedantic $<

program := lzw435
program-objects := lzw435.o

$(program) : $(program-objects)
	g++ $^ -o $@

all: $(program)