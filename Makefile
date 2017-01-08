# -- General -- #
CC       =g++
IDIR     =.
BDIR     =build
# -- Flags/files -- #
CFLAGS =-std=c++11 -I$(IDIR)
ODIR   =obj
OBJ    =main.o quadtree.o
_OBJ   =$(patsubst %,$(ODIR)/%,$(_OBJ))

all: files proj

$(ODIR)/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

proj: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

files:
	mkdir -p $(ODIR)
	mkdir -p build

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o