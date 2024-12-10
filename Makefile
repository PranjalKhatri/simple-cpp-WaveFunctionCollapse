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
all:prog

prog:$(BUILD_DIR)\main.o
	$(CC) -o $(BUILD_DIR)\app.exe $(BUILD_DIR)\main.o $(LINKERFLAGS) 

$(BUILD_DIR)\main.o: $(SOURCE)\main.cpp
	cd $(BUILD_DIR) && $(CC) $(CPPFLAGS) -I $(INCLUDE)  $(SOURCE)\main.cpp
	cd $(LOC)

clean:
	del *.o
