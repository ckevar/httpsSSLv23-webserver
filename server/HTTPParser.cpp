#include "HTTPParser.h"
#include <cstring>

#include <iostream>

// HASH FUNCTIONS BEGIN

// HASH FUNCTIONS END


void HTTPParser::load(char *msg){
	http = msg;		// the head of the message
	http_it = msg; // the iterator
}

void HTTPParser::reset_http_iterator(){
	http_it = http;
}

void HTTPParser::lookup_method(){
	RESTMethod = -1;
	if(memcmp(http_it, HTTP_METHOD_GET_STR, HTTP_METHOD_GET_LEN) == 0) {
		http_it += HTTP_METHOD_GET_LEN + 1; 	// +1, to skip the ' ' (space)
		RESTMethod = HTTP_METHOD_GET;
		return;
	}
	else if (memcmp(http_it, HTTP_METHOD_POST_STR, HTTP_METHOD_POST_LEN) == 0){
		http_it += HTTP_METHOD_POST_LEN + 1; 	// +1, to skip the ' ' (space)
		RESTMethod = HTTP_METHOD_POST;
		return;
	}
	else if (memcmp(http_it, HTTP_METHOD_PUT_STR, HTTP_METHOD_PUT_LEN) == 0) {
		http_it += HTTP_METHOD_PUT_LEN + 1; 	// +1, to skip the ' ' (space)
		RESTMethod = HTTP_METHOD_PUT;
		return;
	}
	else if (memcmp(http_it, HTTP_METHOD_DELETE_STR, HTTP_METHOD_DELETE_LEN) == 0) {
		http_it += HTTP_METHOD_DELETE_LEN + 1; 	// +1, to skip the ' ' (space)
		RESTMethod = HTTP_METHOD_DELETE;
		return;
	}
	else if (memcmp(http_it, HTTP_HTTP_1_1_STR, HTTP_HTTP_1_1_LEN) == 0) {
		http_it += HTTP_HTTP_1_1_LEN + 1; 	// +1, to skip the ' ' (space)
		RESTMethod = HTTP_METHOD_NONE;
		return;
	}
}

void HTTPParser::get_resource() {
	// get the resource
	resource.resource = http_it; // skip the "/"
	resource.length = 0;
	while(*http_it != ' ') {
		http_it++;
		resource.length++;
	}

	// Finish the first line
	while(*http_it != '\n') http_it++;
	// include \n
	http_it++; 
}

void HTTPParser::map_headers() {
	char *tmp_ptr;

	m_header.length = 0;
	tmp_ptr = http_it;
	m_header.length++;
	
	while((m_header.length < HTTP_MAX_HEADERS) && http_it) {
		if ((*http_it == ':') && (*(http_it + 1) == ' ')) {
			m_header.key[m_header.length].str = tmp_ptr;
			m_header.key[m_header.length].length = http_it - tmp_ptr;
			m_header.length++;
			m_header.key[m_header.length].str = nullptr;
			*http_it++; // skips space ": "

		} else if (*http_it == '\n') {

			/* Next header field shall start at the beginning
            of the next line */
			tmp_ptr = http_it + 1;

			// End of headers
			if (memcmp(http_it - 1, "\r\n\r\n", 4) == 0) {
				http_it += 2;
				Content = http_it; /* locate Content Address */
				return;
			}
		} 
		http_it++;
	}
}


char HTTPParser::get_header(const char *headerfield, int length, char **arg) {
	// from experiments, it shows that the first letter and the last two letters 
	// are enough to identify the header
	char *aux;

	for (unsigned short i = 0; i < m_header.length; ++i) {
		// First we check the string length
		if (m_header.key[i].length == length) {
			// then we check the first letter two last ones
			if (*m_header.key[i].str == *headerfield) {
				aux = m_header.key[i].str + length;
				// then the last two letters
				if (*(aux - 1) == *(headerfield + length - 1) &&
					*(aux - 2) == *(headerfield + length - 2)) {
					// the value of the header fields starts after the ": ", two characters
					*arg = aux + 2;
					return 0; 	// found it 
				}
			}
		}
	}
	return 1; // not found 
}

char HTTPParser::parse(){

	// Get REST Method
	lookup_method();
	if(RESTMethod < 0) return -1;

	// Locate Resource
	get_resource();

	// Map header fields pointers
	map_headers();

	// Reset http iterator
	http_it = http;
	return 0;
}
