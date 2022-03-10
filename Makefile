# Usage:
# make			# compile all binary
# make run		# run executable
# make clean		# remove ALL binaries, objects, and executable

# compiler
CXX := g++

# compiler flags
CXXFLAGS := -std=c++11 -pedantic -Wall -Wextra -Wshadow -g

# build target
TARGET := BewareTheBasilisk

$(TARGET): main.o
	@echo "Linking..."
	@$(CXX) main.o -o $(TARGET)
	@echo "Unix executable created : $(TARGET)"

main.o: main.cpp
	@echo "Compiling source files..."
	@$(CXX) -c $(CXXFLAGS) main.cpp

run:
	@./$(TARGET)

.PHONY: clean
clean:
	@$(RM) *.o
	@$(RM) $(TARGET)
