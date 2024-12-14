#a comment
LOC=$(shell cd)
#Compiler
CC=g++
#CFLAGS
CPPFLAGS=-c -Wall -Wno-missing-braces
#INCLUDE PATH
INCLUDE=$(LOC)\Include
#LIBRARY
LIBRARY=$(LOC)\Libs
#BUILD
BUILD_DIR=$(LOC)\Builds
#SOURCE
SOURCE=$(LOC)\Src
#Linker Flags
LINKERFLAGS= -L $(LIBRARY) -lraylib -lopengl32 -lgdi32 -lwinmm

#Object Files
OBJ=$(BUILD_DIR)/main.o 

#default target
all:prog

# Linking Executable
prog: $(OBJ) $(INCLUDE)/*.h
	$(CC) -o $(BUILD_DIR)\app.exe $(OBJ) $(LINKERFLAGS)

# Main Object Compilation
$(BUILD_DIR)/main.o: $(SOURCE)/main.cpp $(INCLUDE)/*.h
	cd $(BUILD_DIR) && $(CC) $(CPPFLAGS) -I $(INCLUDE)  $(SOURCE)/main.cpp
	cd $(LOC)

# Clean Build
clean:
	del $(BUILD_DIR)\*.o
	del $(BUILD_DIR)\app.exe
