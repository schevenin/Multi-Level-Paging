#Sawyer Thompson 
#RedId: 823687079

# CXX Make variable for compiler
CXX=g++ -g
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g -pthread

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
a.out : driver.o tracereader.o output_mode_helpers.o
	$(CXX) $(CXXFLAGS) -o a.out $^
output_mode_helpers.o : output_mode_helpers.cpp
tracereader.o : tracereader.h tracereader.cpp 
driver.o : tracereader.h driver.cpp

clean :
	rm -rf *.o