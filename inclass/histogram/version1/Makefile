histogram.o: histogram.c
	gcc -g -Wall -c histogram.c

test_histogram.o: test_histogram.c
	gcc -g -Wall -c test_histogram.c

test_histogram: test_histogram.o histogram.o
	gcc -o test_histogram test_histogram.o histogram.o -lpthread
