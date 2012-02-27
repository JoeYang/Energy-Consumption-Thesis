CC=g++
CFLAGS=-c -Wall
LDFLAGS=
LOCATIONDIR=$(shell pwd)
SOURCEDIR=$(LOCATIONDIR)/src
BUILDDIR =$(LOCATIONDIR)/build
OUTPUTDIR=$(LOCATIONDIR)/result
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

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(SOURCEDIR)/*.o
	
veryclean:
	rm -rf $(OUTPUTDIR)	

