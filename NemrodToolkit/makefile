CFLAGS=-Wall
CXXFLAGS=-Wall -std=c++11

release: CFLAGS += -O3 -funroll-loops
release: CXXFLAGS += -O3 -funroll-loops
release: all

debug: CFLAGS += -g3 -DDEBUG
debug: CXXFLAGS += -g3 -DDEBUG
debug: all

all: hello.exe generate-overview 

hello.exe: 
	$(CXX) $(CXXFLAGS) hello.cpp -o hello.exe
	
generate-overview: 
	$(CXX) $(CXXFLAGS) Diagnostics.cpp mpfile/MpFile.cpp maptk/ProjectFile.cpp generate-overview.cpp -o generate-overview.exe
	
clean:
	rm -f hello.exe generate-overview.exe *.o 