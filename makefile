CC=g++
CFLAGS=-g -Wall

_DEPS = enigma.h reflector.h rotor.h plugboard.h config.h enum.h errors.h
DEPS = $(patsubst %,%,$(_DEPS))

_OBJ = main.o enigma.o reflector.o rotor.o plugboard.o config.o enum.o
OBJ = $(patsubst %,%,$(_OBJ))

enigma: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean

clean:
	rm -f *.o *~ core *~ 