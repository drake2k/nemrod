CFLAGS=-Wall

all: generate-overview.exe hello.exe

hello.exe: 
	g++ $(CFLAGS) hello.cpp -o hello.exe
	
generate-overview.exe: 
	g++ $(CFLAGS) generate-overview.cpp -o generate-overview.exe
	
clean:
	rm -f generate-overvie.exe *.o 