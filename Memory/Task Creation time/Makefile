all: create_process
	
	create_process: create_process.o
		gcc -o create_process create_process.o
	
	create_process.o: create_process.c
		gcc -c create_process.c
	
	clean:
	rm create_process create_process.o
