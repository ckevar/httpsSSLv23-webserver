
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include <cstring>

#include "WebServer.h"

// Handler for when a message is received from the client
void WebServer::onMessageReceived(SSL *clientSocket, const char *msg, int length) {
	// Parse out the client's request string e.g. GET /index.html HTTP/1.1
	http.load((char *)msg);
	http.parse();

	#ifdef DEBUG
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "->Method " << http.RESTMethod << std::endl;
	std::cout << "->Resource Length " << http.resource.length << std::endl;
	std::cout << "->Resource " << http.resource.resource << std::endl;
	std::cout << "->Content-Length " << http.ContentLength << std::endl;
	#endif

	// Some defaults for output to the client (404 file not found 'page')
	std::string content = "<h1>404 Not Found</h1>";
	std::string requestedFile = "/notfound.html";
	contentType = "text/html";
	int errorCode = 404;

	// TODO: validate header
	// 		Use HOST, or CORS

	// This is checking headers
	// GET Method
	if (http.RESTMethod == HTTP_METHOD_GET) {

		if (http.resource.length == 1) {
			// Client is asking for root resource: "/"
			requestedFile = "/index.html";

			// get "Host"
			char *Host;
			if (http.get_header(HTTP_HOST_STR, HTTP_HOST_LEN, &Host) == 0)
				std::cout << "get_header(\"Host\") " <<  " " << Host << std::endl;
			else 
				std::cerr << "[ERROR] no Host field found" << std::endl;
			
			MIMEType(&requestedFile);
		} 
		// TODO: check other resources
		
	} 
	// POST Method
	else if (http.RESTMethod == HTTP_METHOD_POST){
		if (http.resource.length == 1) {
			std::cout << "content " << http.Content << std::endl;
		}
	}

	std::ostringstream oss;

	// Open the document in the local file system
	std::ifstream f("../wwwroot" + requestedFile);

	// Check if it opened and if it did, grab the entire contents
	if (f.good()) {
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		content = str;
		errorCode = 200;
	}

	f.close();

	// Write the document back to the client
	oss << "HTTP/1.1 " << errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: "<< contentType << "\r\n";
	oss << "Content-Length: " << content.size() << "\r\n";
	oss << "\r\n";
	oss << content;

	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
}

// Handler for client connections
void WebServer::onClientConnected(int clientSocket) {

}

// Handler for client disconnections
void WebServer::onClientDisconnected(int clientSocket) {

}

void WebServer::MIMEType(std::string *rType) {
	int idx = rType->size();
	while (rType->at(idx - 1) != '.') idx--;
	std::string mimetype = rType->substr(idx);

	if (mimetype == "html")
		contentType = "text/html";

	else if (mimetype == "js")
		contentType = "text/javascript";

	else if (mimetype == "css")
		contentType = "text/css";

	else if (mimetype == "jpeg" || mimetype == "jpg")
		contentType = "image/jpeg";

	else if (mimetype == "png")
		contentType = "image/png";

	else if (mimetype == "mp4")
		contentType = "video/mp4";

}
