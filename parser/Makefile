#Nickolas Hoover
#student number: 1024486
#email:hoovern@uoguelph.ca

CFLAGS = -Wall -g -std=c11
BIN = bin/
INC = include/
SRC = src/
XML_PATH = /usr/include/libxml2

# Standard common makefile
parser: $(BIN)libsvgparse.so

$(BIN)libsvgparse.so: $(BIN)SVGParser.o $(BIN)LinkedListAPI.o
	gcc -shared -o $(BIN)libsvgparse.so $(BIN)SVGParser.o $(BIN)LinkedListAPI.o -lxml2 -lm

$(BIN)SVGParser.o: $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser.h
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) $(SRC)SVGParser.c -o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(INC) $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

tester: $(BIN)SVGParser.o $(INC)SVGParser.h $(INC)LinkedListAPI.h $(SRC)LinkedListAPI.c
	gcc $(CFLAGS) -I$(INC) -I$(XML_PATH) $(BIN)SVGParser.o $(BIN)LinkedListAPI.o -o tester -lxml2 -lm	

clean:
	rm $(BIN)*.o $(BIN)*.so
