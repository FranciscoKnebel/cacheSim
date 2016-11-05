EXEC=main
CC=gcc
CFLAGS=-I.
ifeq ($(OS),Windows_NT)
  RM = del
  RRM = rmdir
else
  RM = rm -f
  RRM = rm -f -r
endif

all: $(EXEC) run

main:
	@echo Compilando o projeto.
	$(CC) $(CFLAGS) main.c lib.c -o program

run:
	@echo Executando '.\program':
	@.\program
	@echo Execucao encerrada.

clear:
	cls

clean:
	$(RM) *.exe
