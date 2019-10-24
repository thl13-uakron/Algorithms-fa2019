all: 

# general file compilation rule
%.o: %.cc
	g++ -c $<

# files containing algorithms and helper functions
lib_objects = convexHullAlgs.o convexHullHelpers.o
lib_headers = convexHullAlgs.hh convexHullHelpers.hh
$(lib_objects): $(lib_headers)

# main program
program = convexHull435
program_objects = convexHull435.o
$(program_objects): $(lib_objects) # linking
$(program): $(program_objects)
	g++ $^ -o $@

# driver program to test runtimes for report analysis
test = runtimeTest
test_objects = runtimeTest.o
$(test_objects): $(lib_objects) # linking
$(test): $(test_objects)
	g++ $^ -o $@

clean:
	rm -f $(test) $(program) $(program_objects) $(lib_objects)

.PHONY: test

test: $(test)
	./$(test)

all: $(program)