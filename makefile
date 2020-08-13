server.out client.out:server.cpp client.cpp
	g++ server.cpp -o server.out -pthread -std=c++11
	g++ client.cpp -o client.out -pthread
	
.PHONY:clean
clean:
	rm -rf *.o