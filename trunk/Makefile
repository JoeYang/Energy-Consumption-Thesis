CC=g++
CFLAGS=-c -Wall -w -ansi -pedantic
LDFLAGS=
LOCATIONDIR=$(shell pwd)
TESRDIR=~/Desktop/Project/Optimisation\ Basics/fft/
SOURCEDIR=$(LOCATIONDIR)/src
BUILDDIR =$(LOCATIONDIR)/build
OUTPUTDIR=$(LOCATIONDIR)/result
INSTRUCTIONFILE=$(LOCATIONDIR)/instruction.txt
$(shell [ -d "$(BUILDDIR)" ] || mkdir -p $(BUILDDIR))
$(shell [ -d "$(OUTPUTDIR)" ] || mkdir -p $(OUTPUTDIR))
SOURCE=$(SOURCEDIR)/main.cc 
OBJECTS=$(SOURCE:.cc=.o)
EXECUTABLE=$(BUILDDIR)/run

all: $(SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILDDIR):
	-@ if NOT EXIST "build" mkdir "build"

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

run: $(EXECUTABLE)
	./build/run $(INSTRUCTIONFILE) $(TESRDIR) $(OUTPUTDIR)

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(SOURCEDIR)/*.o
	
cleandata:
	rm -rf $(OUTPUTDIR)	

