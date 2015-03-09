CFLAGS=-Wall
CXXFLAGS=-Wall

release: CFLAGS += -O3 -funroll-loops
release: CXXFLAGS += -O3 -funroll-loops
release: all

debug: CFLAGS += -g3 -DDEBUG
debug: CXXFLAGS += -g3 -DDEBUG
debug: all

all: hello.exe generate-overview.exe 

hello.exe: 
	$(CXX) $(CXXFLAGS) hello.cpp -o hello.exe
	
generate-overview.exe: 
	$(CXX) $(CXXFLAGS) generate-overview.cpp -o generate-overview.exe
	
clean:
	rm -f hello.exe generate-overview.exe *.o 