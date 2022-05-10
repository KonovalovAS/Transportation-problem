run: main.o transportation.o tester_source.o
	g++ main.o transportation.o tester_source.o -o run

main.o: main.cpp
	g++ -c main.cpp

transportation.o: transportation.cpp myheader.h
	g++ -c transportation.cpp

tester_source.o: tester_source.cpp tester.h
	g++ -c tester_source.cpp

clear:
	rm *.o
