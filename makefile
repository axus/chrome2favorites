### Makefile - October 2, 2010 ###

### Program to copy Chrome bookmarks to Windows "Favorites" folder ###
EXE=Chrome2Favorites.exe

# Add application source files here
SRC=googleChromeUtil.cpp sharefile.cpp chrome2favorites.cpp

# Add library paths and files here
LIBS=-L$(HOME)/lib -ljson_mingw_libmt -lshlwapi

# -mconsole: Create a console application
# -mwindows: Create a GUI application
# -Wl,--enable-auto-import: Let the ld.exe linker automatically import from libraries
LDFLAGS= -mconsole -Wl,--enable-auto-import

#Minimum Windows version: Windows XP, IE 6.01
CPPFLAGS= -DMINGW -D_WIN32_WINNT=0x0500 -DWINVER=0x0500 -D_WIN32_IE=0x0601

# Choose object file names from source file names
OBJ=$(SRC:.cpp=.o)

# GNU C++ Compiler
CC=g++

# All warnings, optimization level 3
CFLAGS=-Wall -O3

# Custom header files, MinGW header files
INCLUDES=-I$(HOME)/include -I/mingw/include

# Build object files with chosen options
%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -o $@ -c $<

# Default target of make is "all"
.all: all      
all: $(EXE)

# Build executable from objects and libraries
$(EXE): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $@

# Remove object files and core files with "clean" (- prevents errors from exiting)
RM=rm -f
.clean: clean
clean:
	-$(RM) $(OBJ) core
