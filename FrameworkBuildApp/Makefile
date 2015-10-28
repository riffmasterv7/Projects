#################################################################
## This Makefile Exported by MinGW Developer Studio
## Copyright (c) 2002-2004 by Parinya Thipchart
#################################################################

ifneq (,$(findstring Release, $(CFG)))
  override CFG = Release
else
  override CFG = Debug
endif

PROJECT = Math
CC = D:\Tools\MinGW\bin\gcc.exe

ifeq ($(CFG),Debug)
  OBJ_DIR = ./bin/gcc/Debug
  OUTPUT_DIR = ./bin/gcc/Debug
  TARGET = Math.exe
  C_INCLUDE_DIRS = -I"E:/Projects/Dev/UberTest/src" -I"D:/Tools/MinGW/include" 
  C_PREPROC = -D_DEBUG 
  CFLAGS = -pipe -msse -msse2 -fno-inline -Wall -ggdb2 -O0 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"D:/Tools/MameDevMinGW/mingw64-w32/i686-w64-mingw32/lib" 
  LIBS = 
  LDFLAGS = -pipe -pipe
endif

ifeq ($(CFG),Release)
  OBJ_DIR = ./bin/gcc/Release
  OUTPUT_DIR = ./bin/gcc/Release
  TARGET = Math.exe
  C_INCLUDE_DIRS = -I"E:/Projects/Dev/UberTest/src" -I"D:/Tools/MinGW/include" 
  C_PREPROC = 
  CFLAGS = -pipe -msse -msse2 -finline-functions -Winline -Wall -g0 -O2 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = -L"D:/Tools/MameDevMinGW/mingw64-w32/i686-w64-mingw32/lib" 
  LIBS = 
  LDFLAGS = -pipe -s 
endif

ifeq ($(OS),Windows_NT)
  NULL =
else
  NULL = nul
endif

SRC_OBJS = \
  $(OBJ_DIR)/math.o	\
  $(OBJ_DIR)/matrix.o	\
  $(OBJ_DIR)/test.o	\
  $(OBJ_DIR)/vector.o

define build_target
@echo Linking...
@$(CC) -o "$(OUTPUT_DIR)\$(TARGET)" $(SRC_OBJS) $(LIB_DIRS) $(LIBS) $(LDFLAGS)
endef

define compile_source
@echo Compiling $<
@$(CC) $(CFLAGS) $(C_PREPROC) $(C_INCLUDE_DIRS) -c "$<" -o "$@"
endef

.PHONY: print_header directories

$(TARGET): print_header directories $(SRC_OBJS)
	$(build_target)

.PHONY: clean cleanall

cleanall:
	@echo Deleting intermediate files for 'Math - $(CFG)'
	-@del $(OBJ_DIR)\*.o
	-@del "$(OUTPUT_DIR)\$(TARGET)"
	-@rmdir "$(OUTPUT_DIR)"

clean:
	@echo Deleting intermediate files for 'Math - $(CFG)'
	-@del $(OBJ_DIR)\*.o

print_header:
	@echo ----------Configuration: Math - $(CFG)----------

directories:
	-@if not exist "$(OUTPUT_DIR)\$(NULL)" mkdir "$(OUTPUT_DIR)"
	-@if not exist "$(OBJ_DIR)\$(NULL)" mkdir "$(OBJ_DIR)"

$(OBJ_DIR)/math.o: src/math/math.c

	$(compile_source)

$(OBJ_DIR)/matrix.o: src/math/matrix.c	\
src/math/math.h
	$(compile_source)

$(OBJ_DIR)/test.o: src/math/test.c	\
src/math/math.h
	$(compile_source)

$(OBJ_DIR)/vector.o: src/math/vector.c	\
src/math/math.h
	$(compile_source)

