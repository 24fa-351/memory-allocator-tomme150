test: test_heap_mem.c malloc.c
	gcc -o test test_heap_mem.c malloc.c
	./test

malloc: malloc.c
	gcc -o malloc malloc.c
	./malloc

clean:
	rm test