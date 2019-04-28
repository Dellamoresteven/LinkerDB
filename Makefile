make: system/database.cc system/init.cc system/read.cc system/write.cc
	g++ -o linkerDB system/database.cc system/init.cc system/read.cc system/write.cc

clean:
	rm -f linkerDB
