make: system/database.c
	gcc -o linkerDB system/database.c

clean:
	rm -f linkerDB
