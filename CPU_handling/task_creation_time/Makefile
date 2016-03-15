all: create_process, create_thread	

create_process: create_process.o
		gcc -o create_process create_process.o
	
create_process.o: create_process.c
		gcc -c create_process.c
	
create_thread: create_thread.o
	        gcc -pthread -o create_thread create_thread.c
	         
create_thread.o: create_thread.c
	        gcc -c create_thread.c

clean:
	rm create_process create_process.o
	rm create_thread create_thread.o
