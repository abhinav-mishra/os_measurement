import sys

file = open('testfile.txt', 'w')
print "Setting up a file of ", sys.argv[1], "lines"
for i in range(0,int(sys.argv[1])):
   file.write('a\n')
file.close()
