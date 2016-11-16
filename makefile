EXEC=main
CC=gcc
CFLAGS=-I.
LDFLAGS=-lm

ifeq ($(OS),Windows_NT)
  RM = del
  RRM = rmdir
else
  RM = rm -f
  RRM = rm -f -r
endif

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

all: $(EXEC)

main:
	@echo Compilando o projeto.
	$(CC) $(CFLAGS) main.c lib.c -o cachesim $(LDFLAGS)

run:
	$(CC) $(CFLAGS) main.c lib.c -o cachesim $(LDFLAGS)
	@echo Executando '.\cachesim':
	@echo ./cachesim $(RUN_ARGS)
	@./cachesim $(RUN_ARGS)
	@echo Execucao encerrada.

clear:
	cls

clean:
	$(RM) *.exe
