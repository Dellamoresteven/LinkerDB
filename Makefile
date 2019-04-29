make: system/database.cc system/init.cc system/read.cc system/write.cc
	g++ -std=c++11 -o linkerDB system/database.cc system/init.cc system/read.cc system/write.cc

clean:
	rm -f linkerDB
