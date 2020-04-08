CC = g++
CFLAGS = -Wall -g -std=c++11
LDFLAGS = -lncurses -ltinfo

all: Minesweeper

new:
	make clean
	make

checking: Minesweeper
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./Minesweeper

Minesweeper: main.o game_settings.o IOText.o IOLinux.o
	$(CC) $^ -o $@ $(LDFLAGS) 

main.o: main.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

game_settings.o: GameSettings.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

IOText.o: IOText.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

IOLinux.o: IOLinux.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean
clean:
	rm -f *.o *~ Minesweeper