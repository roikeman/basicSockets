/*
 * basicSockets.h
 *
 *  Created on: Aug 3, 2015
 *      Author: froike
 */
#include <string>
using namespace std;

#ifndef BMRNET_H_
#define BMRNET_H_

#ifdef _WIN32
 #include<winsock2.h>
#endif

class BmrNet {
private:
	char * host;
	unsigned int port;
	bool is_JustServer;
	int socketFd;
	#ifdef _WIN32
	    PCSTR Cport;
		WSADATA wsa;
		DWORD dwRetval;
	#endif


public:
	/**
	 * Constructor for servers and clients, got the host and the port for connect or listen.
	 * After creation call listenNow() or connectNow() function.
	 */
	BmrNet(char * host, int port);

	/**
	 * Constructor for servers only. got the port it will listen to.
	 * After creation call listenNow() function.
	 */
	BmrNet(int portno);

	/**
	 * got data and send it to the other side, wait for response and return it.
	 * return pointer for the data that recived.
	 */
	void* sendAndRecive(const void* data, int get_size, int send_size);

	/**
	 * got data and send it just after receiving someting from the other side
	 * and return what it received.
	 * return pointer for the data that recived.
	 */
	void* reciveAndSend(const void* data, int get_size, int send_size);

	/**
	 * got function fun(void*, int) and after receiving someting from the other side it will call fun(data, get_size)
	 * send the results back to the other side and return what it received.
	 * return pointer for the data that recived.
	 */
	void* reciveAndSend(void* (*fun)(void*,int), int get_size, int send_size);

	virtual ~BmrNet();

	/**
	 * Start listen on the given port.
	 */
	bool listenNow();

	/**
	 * Try to connect to server by given host and port.
	 * return true for success or false for failure.
	 */
	bool connectNow();

	/**
	 * Send Data to the other side.
	 * return true for success or false for failure.
	 */
	bool sendMsg(const void* data, int size);

	/**
	 * Recive data from other side.
	 * return true for success or false for failure.
	 */
	bool reciveMsg(void* buff, int buffSize);


};



#endif /* BMRNET_H_ */
