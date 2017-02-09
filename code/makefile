all: hercules

hercules: main.o Room.o Parser.o Item.o
	g++ -g main.o Parser.o Room.o Item.o -o hercules

main.o: main.cpp Parser.hpp Room.hpp Item.cpp
	g++ -c main.cpp

Item.o: Item.cpp Item.hpp
	g++ -c Item.cpp
	
Room.o: Room.cpp Room.hpp
	g++ -c Room.cpp

Parser.o: Parser.cpp Parser.hpp
	g++ -c Parser.cpp

clean:
	rm -f *.o hercules





