make: system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc
	g++ -std=c++14 -o linkerDB system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc

clean:
	rm -f linkerDB
