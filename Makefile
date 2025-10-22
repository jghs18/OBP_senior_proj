# Makefile for Senior Design ODP
# written by Anthony - Feb 3

CC = g++

CFLAGS = -g -Wall
LDFLAGS =

OBJS = Instruction.o Parser.o networks.o Server.o Client.o packetFormatter.o Unpacker.o checksum.o pollLib.o TeleData.o

all: sender test

sender: sender.cpp $(OBJS)
	$(CC) $(CFLAGS) -o sender sender.cpp $(OBJS) $(LIBS)

recver: recver.cpp $(OBJS)
	$(CC) $(CFLAGS) -o recver recver.cpp $(OBJS) $(LIBS)

test: tester.cpp $(OBJS)
	$(CC) $(CFLAGS) -o test tester.cpp $(OBJS) $(LIBS)

#SC: spaceCraft.cpp $(OBJS)
#	$(CC) $(CFLAGS) -o SC spaceCraft.cpp $(OBJS) $(LIBS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

cleano:
	rm -f *.o

clean:
	rm -f bash *.o