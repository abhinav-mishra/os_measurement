import os
import sys
import linecache
import random
import socket
from linereader import getline



s = socket.socket()
host = socket.gethostname()
port = 1234
s.bind((host,port))
s.listen(5)
while True:
    c,addr = s.accept()
    filename = c.recv(1024)
    print("received ", filename)
    c.send("received "+filename)
    position = int(c.recv(1024))
    c.send("received "+ str(position))   
    #linecache.clearcache()
    newl = getline('testfile.txt',position)
    print newl
    c.send(newl)
    c.close()
