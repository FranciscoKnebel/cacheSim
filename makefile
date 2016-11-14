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
	$(CC) $(CFLAGS) main.c lib.c -o program -lm

run:
	@echo Executando '.\program':
	@echo ./program $(RUN_ARGS)
	@./program $(RUN_ARGS)
	@echo Execucao encerrada.

clear:
	cls

clean:
	$(RM) *.exe
