#include "YASL.h"      // For Socket and SocketException
#include "checkArgs.h"
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include <fstream>     // For File Stream

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer

int main(int argc, char *argv[]) {

	checkArgs* argumentos = new checkArgs(argc, argv);

	std::string servAddress; 
	uint16_t    ServPort;
	std::string httpRequest;
	std::string archivo;
	
	servAddress   = argumentos->getArgs().SERVER;
	ServPort      = argumentos->getArgs().PORT;
	httpRequest   = "GET / HTTP/1.1\r\nHOST: " + argumentos->getArgs().SERVER + "\r\n\r\n";
	archivo = argumentos->getArgs().FILE;
	delete argumentos;
	
	uint32_t requestLen = httpRequest.length();   // Determine input length
	try {
	    // Create or open the file
		std::ofstream fs ;
		fs.open(archivo);
		// Establish connection with the echo server
		TCPSocket sock(servAddress, ServPort);
		// Send the request to the server
		sock.send(httpRequest.c_str(), requestLen);
		char Buffer[RCVBUFSIZE + 1];    // Buffer for string + \0
		uint32_t bytesReceived = 1;              // Bytes read on each recv()
		uint32_t totalBytesReceived = 0;         // Total bytes read
		
		std::cout << "Received: " << std::endl;               // Setup to print the responce string
		while (bytesReceived!=0) {
			// Receive up to the buffer size bytes from the sender
			if ((bytesReceived = (sock.recv(Buffer, RCVBUFSIZE))) < 0) {
				std::cerr << "Unable to read";
				fs.close () ;
				exit(EXIT_FAILURE);
			}
			totalBytesReceived += bytesReceived;     // Keep tally of total bytes
			Buffer[bytesReceived] = '\0';        // Terminate the string!
			fs << Buffer << std::endl;           // Print on the file
		}
		fs << std::endl;
		fs.close ();

		// Destructor closes the socket

	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
