CXX := g++

# Compiler flags
CXXFLAGS := -Wall -Werror -g -std=c++17

# Output binary
TARGET := main

# Object files
OBJS := \
	main.o \
	data.o \
	operations.o \
	shop.o \
	pokemud/pokemon.o \
	pokemud/ui.o \
	pokemud/move.o \
	pokemud/json11.o \
	pokemud/battle.o \
	pokemud/load_pson.o \
	pokemud/team.o \
	pokemud/gym.o

# Default target (what Replit runs)
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Header dependencies
data.o: data.h
operations.o: operations.h
shop.o: shop.h

pokemud/json11.o: pokemud/json11.hpp
pokemud/ui.o: pokemud/ui.h
pokemud/pokemon.o: pokemud/pokemon.h
pokemud/move.o: pokemud/move.h
pokemud/battle.o: pokemud/battle.h
pokemud/load_pson.o: pokemud/load_pson.h
pokemud/team.o: pokemud/team.h

# Clean build artifacts
clean:
	find . -type f \( -name '*.o' -o -name '*.gcno' -o -name '*.gcda' \) -delete
	rm -f $(TARGET)

.PHONY: all clean