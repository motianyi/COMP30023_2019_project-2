CC = gcc
CFLAGS = -std=c99 -O3 -Wall -Wpedantic

EXE = crack
OBJ = sha256.o crack.o 


$(EXE): $(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ)
main.o: crack.c
		$(CC) $(CFLAGS) -c crack.c
sha256.o: sha256.c 
		$(CC) $(CFLAGS) -c sha256.c

clean:
	rm -f $(OBJ) $(EXE)

