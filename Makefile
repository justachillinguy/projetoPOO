CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude
SRC = $(wildcard src/*.cpp) main.cpp
TARGET = bin/Debug/ProjetoPOO.exe

ifeq ($(OS),Windows_NT)
    MKDIR = if not exist bin\Debug mkdir bin\Debug
    RM = if exist bin\Debug\ProjetoPOO.exe del /Q bin\Debug\ProjetoPOO.exe
else
    MKDIR = mkdir -p bin/Debug
    RM = rm -f $(TARGET)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	@$(RM)
