CC = gcc
CFLAGS = -std=c99 -O3 -Wall -Wpedantic

EXE = dh
OBJ = sha256.o dh.o 

EXE1 = crack
OBJ1 = sha256.o crack.o 

$(EXE): $(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

$(EXE1): $(OBJ1)
		$(CC) $(CFLAGS) -o $(EXE1) $(OBJ1)

crack.o: crack.c
		$(CC) $(CFLAGS) -c crack.c

dh.o: dh.c
		$(CC) $(CFLAGS) -c dh.c
sha256.o: sha256.c 
		$(CC) $(CFLAGS) -c sha256.c

clean:
	rm -f $(OBJ) $(EXE) $(EXE1) $(OBJ1)









