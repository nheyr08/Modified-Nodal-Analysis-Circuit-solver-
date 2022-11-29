output: main.o
	gcc main.o -o output

main.o: main.c  misc_functions.h list.h hashing.h net_node_class.h parser.h solving_ax_b.h
	gcc -c main.c

clean:
	rm *.o output