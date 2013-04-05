CC = gcc
# CC = g++

CFLAGS = -Wall -o3 -g

headers = cpu_md5.h

objects = main.o cpu_md5.o md5.o buffer.o

target: cpu_md5 encryptor_md5

encryptor_md5: md5_wiki.cpp
	gcc -g -O3 -o $@ $<

cpu_md5: $(objects) 
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -lstdc++

%.o: %.c $(headers)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f cpu_md5 encryptor_md5 *.o

#clean3:
#	ls -l | grep -i "rwx" | awk '{print "rm " $9}' | bash
