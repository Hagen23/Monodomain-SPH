CC = g++
CFLAGS =-std=gnu++0x -Wall -Ofast
MATH3D =Math3D/
INCLUDES =-I Math3D/
DEBUGF =#-ggdb -g -O0
DFLAGS =$(CFLAGS) -ggdb

all: MemFH.o PointStim.o MonodomainSPH.o MonodomainFD.o Output.o Main.o run

MemFH.o: MemFH.cc MemFH.h Types.h Constants.h
	${CC} ${DEBUGF} ${CFLAGS} -c MemFH.cc

PointStim.o: PointStim.cc PointStim.h Types.h Constants.h
	${CC} ${DEBUGF} ${CFLAGS} -c PointStim.cc

MonodomainSPH.o: MonodomainSPH.cc MonodomainSPH.h Constants.h
	${CC} ${DEBUGF} ${CFLAGS} ${INCLUDES} -c MonodomainSPH.cc

MonodomainFD.o: MonodomainFD.cc MonodomainFD.h Types.h Constants.h
	${CC} ${DEBUGF} ${CFLAGS} -c MonodomainFD.cc

Output.o: Output.cc Output.h Types.h Constants.h
	${CC} ${DEBUGF} ${CFLAGS} -c Output.cc

Main.o: Main.cc MemFH.h MonodomainFD.h Types.h Constants.h
	${CC} ${DEBUGF} ${CFLAGS} -c ${INCLUDES} Main.cc

run: MemFH.o PointStim.o MonodomainSPH.o MonodomainFD.o Output.o Main.o
	${CC} ${DEBUGF} ${CFLAGS} MemFH.o PointStim.o MonodomainSPH.o MonodomainFD.o Output.o Main.o -o run

clean:
	rm -rf *o run

