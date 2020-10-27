CC = g++
CFLAGS = -Wall -O0 -g

INCL = -I ./include

DEPS := $(shell ls ./include/ | grep '.h$$' | sed -e 's/^/.\/include\//')

OBJ := $(shell ls ./src | grep '.cpp$$' | sed -e 's/.cpp$\/.o/')

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: src/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCL)

clean:
	rm -f $(OBJ) main
