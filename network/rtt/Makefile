ITERATIONS=1000
#RADDR="128.54.47.226"
RADDR="127.0.0.1"
RPORT="7"
#RPORT="17600"

# Default Ping Packet data size is 56
# We also want our data size to match in order to 
# perform a comparison of RTT between TCP and Ping (ICMP).
DATA_BYTES=56

all: rtt

rtt: rtt.o
	gcc -o rtt rtt.o

rtt.o: rtt.c
	gcc -O0 -c rtt.c -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS) -D DATA_BYTES=$(DATA_BYTES)

clean:
	rm rtt rtt.o
