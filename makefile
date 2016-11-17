EXEC=main
CC=gcc
CFLAGS=-I.
LDFLAGS=-lm -std=c99

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
	$(CC) $(CFLAGS) main.c preprocess.c process.c -o cachesim $(LDFLAGS)

run:
	@echo Executando '.\cachesim':
	@echo ./cachesim $(RUN_ARGS)
	@./cachesim $(RUN_ARGS)
	@echo Execucao encerrada.

proto:
	$(CC) $(CFLAGS) main.c preprocess.c process.c -o cachesim $(LDFLAGS)
	@echo Executando '.\cachesim':
	@echo ./cachesim test/cachedesc_A.dat test/input_A.dat test/output_A.dat
	@./cachesim test/cachedesc_A.dat test/input_A.dat test/output_A.dat
	@echo Execucao encerrada.

clear:
	cls

clean:
	$(RM) *.exe
