# -- General -- #
CXX       =g++
IDIR     =.
BDIR     =build
# -- Flags/files -- #
CXXFLAGS =-std=c++11 -I$(IDIR) -g -static-libgcc -static -static-libstdc++
ODIR   =obj
OBJ    =main.o quadtree.o
_OBJ   =$(patsubst %,$(ODIR)/%,$(_OBJ))

all: files quadtree

$(ODIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

quadtree: $(OBJ)
	$(CXX) -o $(BDIR)/$@ $^ $(CXXFLAGS)

files:
	mkdir -p $(ODIR)
	mkdir -p build

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f $(BDIR)/*
	rm *.o