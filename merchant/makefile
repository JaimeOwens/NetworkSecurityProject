CPP = g++
CFLAGS = -g -Wall
TAGGET = main

$(TARGET):main.o SHA512.o mblock.o rsa.o
	$(CPP) main.o SHA512.o mblock.o rsa.o -o main

main.o:main.cpp merchant.h
	$(CPP)  -c main.cpp -o main.o 

SHA512.o:../sha-512/SHA512.cpp ../sha-512/SHA512.h
	$(CPP)  -c ../sha-512/SHA512.cpp

mblock.o:../sha-512/mblock.cpp ../sha-512/mblock.cpp
	$(CPP)  -c ../sha-512/mblock.cpp 

rsa.o:../RSA/rsa.cpp ../RSA/rsa.h
	$(CPP)  -c ../rsa/rsa.cpp 

clean:
	rm -rf *.o $(TARGET)
