all: system_call

system_call: system_call.o
	gcc -o system_call system_call.o

system_call.o: system_call.c
	gcc -c system_call.c

clean:
	rm system_call system_call.o
