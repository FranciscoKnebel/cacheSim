# CacheSim

### Compiler: GCC 6.2
```
	Makefile:

	make
		build files and run

	make run ARGS
		run built program

	make clean
		clean *.exe
```

ARGS:
- cache_description.dat input.dat output.dat

## Replacement Policies

### LRU
Least Recently Used

lastAccess no bloco.

##### Set não está cheio:

	Inserção de elemento:
		Ao inserir um elemento, lastAccess recebe o valor 1, pois é o mais recente.
		Todos os outros elementos válidos devem incrementar o seu valor em 1.

	Modificação de elemento:
		Já existe um bloco com essa tag no set, logo não é necessário uma inserção.
		Salvar lastAccess do elemento. Elemento alterado recebe lastAccess com valor 1, pois é o mais recente.
		Os elementos válidos com lastAccess menor do que o antigo desse elemento são incrementados em um.
		Os elementos válidos com lastAccess maior permanecem com o mesmo valor de antes.

##### Set está cheio:

	Inserção de elemento:
		O set está completamente cheio, e está sendo pedido a inserção de um elemento nessa lista.
		Será necessário retirar o elemento com o maior lastAccess da lista, que deve ter valor igual à lineSize * associativity.
		Alterar os elementos desse bloco para os valores do novo elemento, e alterar o lastAccess para 1.
		Todos os outros elementos agora deverão ser incrementados em 1, pois não há valores com lastAccess maior do que o antigo.

	Modificação de elemento:
		Mesmo procedimento do caso Set não está cheio.


### FIFO
First In First Out

orderInsert no bloco.

##### Set não está cheio:

	Inserção de elemento:
		Ao inserir um elemento, orderInsert recebe o valor 1, pois é o mais recente.
		Todos os outros elementos válidos devem incrementar o seu valor em 1.

	Modificação de elemento:
		Já existe um bloco com essa tag no set, logo não é necessário uma inserção.
		Não foi inserido um elemento, logo não é necessário modificar orderInsert do set.

##### Set está cheio:

	Inserção de elemento:
		O set está completamente cheio, pedindo uma inserção de elemento na lista.
		O elemento com maior orderInsert (que deve ser igual à lineSize * associativity) do set será removido da lista,
		pois ele foi o "primeiro" a ser inserido, e o "primeiro" que deve ser removido.
		Incrementar em um o valor de orderInsert de todos os elementos do set, e o maior será substituído
		pelo novo elemento, recebendo orderInsert 1.

	Modificação de elemento:
		Mesmo procedimento do caso Set não está cheio.

-----

## Authors

* **Francisco Knebel** - *Initial work* - [FranciscoKnebel](https://github.com/franciscoknebel)

See also the list of [contributors](https://github.com/franciscoknebel/cacheSim/contributors) who participated in this project.


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
