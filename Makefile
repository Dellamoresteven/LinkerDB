make: system/database.cc
	g++ -o linkerDB system/database.cc

clean:
	rm -f linkerDB
