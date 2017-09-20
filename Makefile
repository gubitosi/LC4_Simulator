all: trace

trace: trace.c ObjectFiles.o LC4.o binary.o
	clang -o trace trace.c -lm ObjectFiles.o LC4.o binary.o

LC4.o: LC4.h LC4.c
	clang -c LC4.c

ObjectFiles.o: ObjectFiles.h ObjectFiles.c
		clang -c ObjectFiles.c

binary.o: binary.h binary.c
	clang -c binary.c

clean: 
	rm -rf *.o

clobber: clean
	rm -rf trace

 
