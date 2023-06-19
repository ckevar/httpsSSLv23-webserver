CPP = g++

main: main.cpp TcpListener.cpp HTTPParser.cpp WebServer.cpp
	$(CPP) -o main main.cpp TcpListener.cpp HTTPParser.cpp WebServer.cpp -lssl -lcrypto -Wall

debug: main.cpp TcpListener.cpp HTTPParser.cpp WebServer.cpp
	$(CPP) -o main_debug main.cpp TcpListener.cpp HTTPParser.cpp WebServer.cpp -lssl -lcrypto -Wall -D DEBUG

clear:
	rm main main_debug