############################# Makefile ##########################
# AUTHOR:   Samuel Terra Vieira                                 #
# ADDRESS:  Universidade Federal de Lavras                      #
# DATE:     23/09/2019                                          #
#################################################################

#################################################################
#                     Variable definition                       #
#################################################################
SHELL:=/bin/bash

# Object directories recursive
OBJ_DIRS=$(shell find ./source/ -type d | cut -c 10- | tail -n+2)

# .c folder with files
C_SOURCE=$(shell find ./source/ -type f -name '*.c')

# .h folder with files
H_SOURCE=$(shell find ./source/ -type f -name '*.h')

# Object files
OBJ=$(subst .c,.o,$(subst source,objects,$(C_SOURCE)))

# Compiler and linker
CC=gcc

# Flags for compiler
CC_FLAGS=-c         \
#         -Wall      \
#         -W         \
#         -ansi      \
#         -pedantic

# Command used at clean target
RM = rm -rf

#################################################################
#                    Compilation and linking                    #
#################################################################

all: objFolder $(OBJ_DIRS) $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@ echo ' '
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./source/%.c ./source/%.h
	@ echo ' '
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

./objects/main.o: ./source/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

$(OBJ_DIRS):
	@ mkdir -p ./objects/$@

clean:
	@ $(RM) ./objects/ $(PROJECT_NAME) *~

organize:
	@ $(shell mkdir -p source/)
	@ $(shell ls | grep -v Makefile | grep -v source | xargs -I{} mv ./{} ./source/)
	@ echo 'Organized project. You can now compile it.'

run: $(PROJECT_NAME)
	./$(PROJECT_NAME)

.PHONY: all clean
