import socket
import sys
import random
import linecache
import linereader
from linereader import getline



filesize = int(sys.argv[1])
numreads = int(sys.argv[3])

if sys.argv[2] == 's':                             ## argv[2] is 's' or 'r' for sequential or random access
    position=random.randint(1,filesize-numreads-1)
    for reps in range(0, numreads):         ## argv[3] is the number of reads to do
        linecache.clearcache()
	position = position+reps        ##argv[1] is the number of lines in the file
	s = socket.socket()
	host = socket.gethostname()
	port = 1234
	s.connect((host,port))
	s.send('testfile.txt')
	ack1=s.recv(1024)
	#print(ack1)
	s.send(str(position))
	ack2 = s.recv(1024)

elif sys.argv[2] == 'r':
   for reps in range(0, numreads):
        linecache.clearcache()
        position = random.randint(1,filesize-1)
        s = socket.socket()
        host = socket.gethostname()
        port = 1234
        s.connect((host,port))
        s.send('testfile.txt')
        ack1=s.recv(1024)
        #print(ack1)
        s.send(str(position))
        ack2 = s.recv(1024)

elif sys.argv[2]=='xs':
  file = 'testfile.txt'
  startpos = position=random.randint(1,filesize-numreads-1)

  for reps in range(0, numreads):
        linecache.clearcache()
        position = startpos+reps
        linestr = getline('testfile.txt',position)
        if (reps>2000): print(line)

elif sys.argv[2]=='xr':
  file = 'testfile.txt'
  pos = random.randint(1,filesize-1)
  for reps in range(0, numreads):
        linecache.clearcache()
        linestr = getline('testfile.txt',pos)
        if (reps>2000): print(line)


