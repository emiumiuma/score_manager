CC=gcc
BIN=score_manager.bin
STD=-std=gnu99
FLAG=-Wall -Werror 
OBJ=login.o main.o principal.o student.o teacher.o tools.o

all:$(OBJ)
	$(CC) -o $(BIN) $(OBJ)
main.o:main.c login.c login.h tools.c tools.h
	$(CC) $(STD) $(FLAG) -c main.c
login.o:login.c login.h principal.c principal.h student.c student.h teacher.c teacher.h tools.c tools.h
	$(CC) $(STD) $(FLAG) -c login.c
principal.o:principal.c principal.h teacher.c teacher.h tools.c tools.h
	$(CC) $(STD) $(FLAG) -c principal.c
student.o:student.c student.h tools.c tools.h
	$(CC) $(STD) $(FLAG) -c student.c
teacher.o:teacher.c teacher.h student.c student.h tools.c tools.h
	$(CC) $(STD) $(FLAG) -c teacher.c
tools.o:tools.c tools.h teacher.c teacher.h principal.c principal.h student.c student.h login.c login.h
	$(CC) $(STD) $(FLAG) -c tools.c

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf *~
