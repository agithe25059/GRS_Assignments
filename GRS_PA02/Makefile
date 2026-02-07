# Roll No: MT25059
# Makefile for PA02

CC = gcc
CFLAGS = -Wall -pthread
TARGETS = MT25059_Part_A1_Server MT25059_Part_A1_Client \
          MT25059_Part_A2_Server \
          MT25059_Part_A3_Server

all: $(TARGETS)

MT25059_Part_A1_Server: MT25059_Part_A1_Server.c MT25059_common.h
	$(CC) $(CFLAGS) -o MT25059_Part_A1_Server MT25059_Part_A1_Server.c

MT25059_Part_A1_Client: MT25059_Part_A1_Client.c MT25059_common.h
	$(CC) $(CFLAGS) -o MT25059_Part_A1_Client MT25059_Part_A1_Client.c

MT25059_Part_A2_Server: MT25059_Part_A2_Server.c MT25059_common.h
	$(CC) $(CFLAGS) -o MT25059_Part_A2_Server MT25059_Part_A2_Server.c

# Client A2 is effectively Client A1 (Standard Recv), so we don't need a separate binary
# But if you created a file MT25059_Part_A2_Client.c, add it here.

MT25059_Part_A3_Server: MT25059_Part_A3_Server.c MT25059_common.h
	$(CC) $(CFLAGS) -o MT25059_Part_A3_Server MT25059_Part_A3_Server.c

clean:
	rm -f $(TARGETS) *.o *.csv *.pdf *.txt *.png
