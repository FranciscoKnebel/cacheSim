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
	@echo --- cacheSim ---
	@echo Francisco Knebel - 243688
	@echo Compilando o projeto.
	@echo ---          ---
	$(CC) $(CFLAGS) main.c preprocess.c process.c -o 243688 $(LDFLAGS)

run:
	@echo Executando '.\243688':
	@echo ./243688 $(RUN_ARGS)
	@./243688 $(RUN_ARGS)
	@echo Execucao encerrada.

tests:
	$(CC) $(CFLAGS) main.c preprocess.c process.c -o 243688 $(LDFLAGS)
	@echo Executando '.\243688':
	@echo ./243688 test/cachedesc_A.dat test/input_A.dat test/output_A.dat
	@./243688 test/cachedesc_A.dat test/input_A.dat test/output_A.dat
	@echo ./243688 test/cachedesc_B.dat test/input_B.dat test/output_B.dat
	@./243688 test/cachedesc_B.dat test/input_B.dat test/output_B.dat
	@echo ./243688 test/cachedesc_C.dat test/input_C.dat test/output_C.dat
	@./243688 test/cachedesc_C.dat test/input_C.dat test/output_C.dat
	@echo ./243688 test/cachedesc_D.dat test/input_D.dat test/output_D.dat
	@./243688 test/cachedesc_D.dat test/input_D.dat test/output_D.dat
	@echo Execucao encerrada.
	

clear:
	cls

clean:
	$(RM) *.exe
