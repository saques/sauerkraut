all: sauerkraut

OBJS = sauerkraut.o  \
		tokens.o \
		main.o \
		codegen.o

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf sauerkraut.cpp sauerkraut.hpp sauerkraut tokens.cpp $(OBJS)

sauerkraut.cpp: sauerkraut.y
	bison -d --debug --verbose  -o $@ $^

sauerkraut.hpp: sauerkraut.cpp

tokens.cpp: tokens.l sauerkraut.hpp
	flex -o $@ $^

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<


sauerkraut: $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

test: sauerkraut example.txt
	cat example.txt | ./sauerkraut
