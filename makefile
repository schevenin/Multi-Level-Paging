# Programmers: Rogelio Schevenin, Sawyer Thompson 
# RedID: 824107681, 823687079

# CXX Make variable for compiler
CXX=g++ -g

# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

a.out : driver.o tracereader.o output_mode_helpers.o pagetable.o level.o
	$(CXX) $(CXXFLAGS) -o a.out $^
output_mode_helpers.o : output_mode_helpers.cpp
tracereader.o : tracereader.h tracereader.cpp 
pagetable.o: tracereader.h pagetable.h pagetable.cpp
level.o : level.h level.cpp
driver.o : tracereader.h driver.cpp

clean :
	rm -rf *.o