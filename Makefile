make: system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc
	g++ -std=c++17 -o Linker system/database.cc system/init.cc system/read.cc system/write.cc system/tableLookup.cc system/print.cc system/seperateCommand.cc system/response.cc server/server.cc

server: server/server.cc
	g++ -std=c++17 -o ser server/server.cc

client: testClient/Client1.cc
	g++ -std=c++17 -o cli1 testClient/Client1.cc

clean:
	rm -f linkerDB
	rm -f cli1
	rm -f ser
	rm -f Linker
