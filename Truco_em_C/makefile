main: cliente.o
	gcc cliente.o -o cliente

cliente.o:	cliente.c cliente.h
	gcc -c cliente.c -o cliente.o

servidor.o: servidor.c servidor.h
	gcc -c servidor.c -o servidor.o

usuario.o: usuario.c usuario.h
	gcc -c usuario.c -o usuario.o

clean:
	rm	-rf	*.o	main