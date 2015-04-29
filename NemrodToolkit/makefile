# take a look at http://stackoverflow.com/questions/5273374/make-file-with-source-subdirectories
# if planning to not use autotools or CMake

# todo static link libstdc++ and libgcc++
# todo implement LDFLAGS and set the '-s' flag while linking

CFLAGS=-Wall
CXXFLAGS=-Wall -std=gnu++11

# must match source tree
_OBJ = Diagnostics.o Utils.o mpfile/MpFile.o  maptk/ProjectFile.o Geodata.o
ODIR=obj
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
HELLO_OBJ=$(ODIR)/hello.o
GENERATE_OVERVIEW_OBJ=$(ODIR)/generate-overview.o

.PHONY: release debug all clean

release: CFLAGS += -O3 -funroll-loops
release: CXXFLAGS += -O3 -funroll-loops
release: all

debug: CFLAGS += -g3 -DDEBUG
debug: CXXFLAGS += -g3 -DDEBUG
debug: all
	
all: generate-overview.exe hello.exe

$(ODIR)/%.o : ./%.cpp ./%.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
#define compile_rule
#$(ODIR)/%.o : $1/%.cpp
#	mkdir -p $$(ODIR)/$1
#	$$(CXX) $$(CXXFLAGS) -c -o $$@ $$<
#endef    
#$(foreach SUBDIR,$(SRC_DIRS),$(eval $(call compile_rule,$(SUBDIR))))

$(HELLO_OBJ) : hello.cpp
	mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(GENERATE_OVERVIEW_OBJ) : generate-overview.cpp
	mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

generate-overview.exe: $(OBJ) $(GENERATE_OVERVIEW_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

hello.exe: $(OBJ) $(HELLO_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ 

clean:
	rm -rf hello.exe generate-overview.exe obj