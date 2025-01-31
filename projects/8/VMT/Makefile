CC=g++
CFLAGS=-Wall -c -std=c++11 -g

VMTranslator: Parser.o CodeWriter.o VMTranslator.o main.o
	$(CC) -Wall -std=c++11 -I. -o VMTranslator Parser.o CodeWriter.o VMTranslator.o main.o -g
	chmod ugo+x ./VMTranslator
Parser.o: Parser.cpp Parser.hpp
	$(CC) $(CFLAGS) Parser.cpp

CodeWriter.o: CodeWriter.cpp CodeWriter.hpp
	$(CC) $(CFLAGS) CodeWriter.cpp

VMTranslator.o: VMTranslator.cpp VMTranslator.hpp Parser.hpp CodeWriter.hpp
	$(CC) $(CFLAGS) VMTranslator.cpp

main.o: main.cpp VMTranslator.hpp
	$(CC) $(CFLAGS) main.cpp

.PHONY: clean
clean:
	rm -rf *.o VMTranslator