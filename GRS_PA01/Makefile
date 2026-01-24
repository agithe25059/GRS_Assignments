
CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread -lm

all: program_a program_b

program_a: MT25059_Part_A_Program_A.c MT25059_Part_B_worker.h
	$(CC) $(CFLAGS) MT25059_Part_A_Program_A.c -o program_a $(LDFLAGS)

program_b: MT25059_Part_A_Program_B.c MT25059_Part_B_worker.h
	$(CC) $(CFLAGS) MT25059_Part_A_Program_B.c -o program_b $(LDFLAGS)

clean:
	rm -f program_a program_b MT25059_Part_C_CSV.csv MT25059_Part_D_CSV.csv *.dat