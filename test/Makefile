main : *.o
	g++ -o main *.o
	rm -f *.o
*.o : *.cpp
	g++ -c *.cpp -std=c++11
clean : 
	rm -f ./*.o main

