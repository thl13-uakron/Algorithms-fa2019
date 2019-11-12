all:

%.o: %.cp
	g++ -c -O2 -Wall -Wextra -pedantic $<

# normal version
program := lzw435
program-objects := lzw435.o

$(program) : $(program-objects)
	g++ $^ -o $@

# modified version
program-m := lzw435m
program-m-objects := lzw435m.o

$(program-m) : $(program-m-objects)
	g++ $^ -o $@

m: $(program-m)

all: $(program) $(program-m)