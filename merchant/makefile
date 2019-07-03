CPP = g++
CFLAGS = -g -Wall
TAGGET = main

$(TARGET):main.o SHA512.o mblock.o rsa.o
	$(CPP) main.o SHA512.o mblock.o rsa.o -o main

main.o:main.cpp merchant.h
	$(CPP)  -c main.cpp -o main.o 

SHA512.o:../sha-512/SHA512.cpp ../sha-512/SHA512.h
	$(CPP)  -c ../sha-512/SHA512.cpp -std=c++11

mblock.o:../sha-512/mblock.cpp ../sha-512/mblock.cpp
	$(CPP)  -c ../sha-512/mblock.cpp -std=c++11

rsa.o:../RSA/rsa.cpp ../RSA/rsa.h
	$(CPP)  -c ../rsa/rsa.cpp -std=c++11

clean:
	rm -rf *.o $(TARGET)
