make: system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc
	g++ -std=c++14 -o linkerDB system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc

server: server/server.cc
	g++ -std=c++14 -o ser server/server.cc

client: testClient/Client1.cc
	g++ -std=c++14 -o cli1 testClient/Client1.cc

# Need this to compile everyhting, Can only have 1 main
all:
	g++ -std=c++14 -o Linker system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc testClient/Client1.cc server/server.cc

clean:
	rm -f linkerDB
	rm -f cli1
	rm -f ser
