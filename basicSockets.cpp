/*
 * basicSockets.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: froike
 */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "basicSockets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>

#define bufferSize 256

#ifdef __linux__
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#define Sockwrite(sock, data, size) write(sock, data, size) 
	#define Sockread(sock, buff, bufferSize) read(sock, buff, bufferSize)
	#define socklen_t unsigned int
#elif _WIN32
	//#pragma comment (lib, "ws2_32.lib") //Winsock Library
	#pragma comment (lib, "Ws2_32.lib")
	//#pragma comment (lib, "Mswsock.lib")
	//#pragma comment (lib, "AdvApi32.lib")
	#include<winsock.h>
	//#include <ws2tcpip.h>
	#define socklen_t int
	#define close closesocket
	#define Sockwrite(sock, data, size) send(sock, (char*)data, size, 0)
	#define Sockread(sock, buff, bufferSize) recv(sock, (char*)buff, bufferSize, 0)
	
#endif





using namespace std;

BmrNet::BmrNet(char* host, int portno) {
	this->port = portno;
#ifdef _Win32
	this->Cport = (PCSTR)portno;
#endif
	this->host = host;
	this->is_JustServer = false;
	this->socketFd = NULL;
	#ifdef _WIN32
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Failed. Error Code : %d\n", WSAGetLastError());
		}else printf("WSP Initialised.\n");
	#endif

}

BmrNet::BmrNet(int portno) {
	this->port = portno;
	this->host = "";
	this->is_JustServer = true;
	this->socketFd = NULL;
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d\n", WSAGetLastError());
	}
	else printf("WSP Initialised.\n");
#endif
}

BmrNet::~BmrNet() {
	close(socketFd);
	#ifdef _WIN32
		WSACleanup();
	#endif
	printf("Closeing connection\n");
}

bool BmrNet::listenNow(){
	int serverSockFd;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;


	serverSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSockFd < 0){
		cout<<"ERROR opening socket"<<endl;
		return false;
	}
	memset(&serv_addr, 0,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(this->port);
	if (bind(serverSockFd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0){
		cout<<"ERROR on binding"<<endl;
		return false;
	}
	listen(serverSockFd,5);
	clilen = sizeof(cli_addr);
	printf("Start listening!");
	this->socketFd = accept(serverSockFd, (struct sockaddr *) &cli_addr,
			&clilen);
	if (this->socketFd < 0){
		cout<<"ERROR on accept"<<endl;
		return false;
	}

	printf("Connected!");

	close(serverSockFd);
	return true;
}


bool BmrNet::connectNow(){
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int n;

	if (is_JustServer){
		cout<<"ERROR: Never got an host... please use the secend constractor"<<endl;;
		return false;
	}

	//fprintf(stderr,"usage %s hostname port\n", host);
	socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketFd < 0){
		cout<<("ERROR opening socket")<<endl;
		return false;
	}

	

	/*server = gethostbyname(host);
	if (server == NULL) {	
		fprintf(stderr,"ERROR, no such host\n");
		return false;
	}*/

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr(host);  //host);
	serv_addr.sin_port			= htons(port);  // port);
	//memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	if (connect(socketFd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		#ifdef _WIN32
			cout << "ERROR connect() - " << WSAGetLastError()<<"!!!\n"<<endl;
		#else
			cout << "ERROR connect()"<<endl;
		#endif
		return false;
	}


	printf("Connected!");

	return true;

}


void* BmrNet::sendAndRecive(const void* data, int get_size, int send_size){

	char * buffer = new char[get_size];
	sendMsg(data, send_size);
	reciveMsg(buffer, get_size);
	return buffer;
}

void* BmrNet::reciveAndSend(void* (*fun)(void*,int), int get_size, int send_size){
	char * buffer = new char[get_size];
	reciveMsg(buffer,get_size);
	sendMsg( fun(buffer, send_size) ,send_size );
	return buffer;
}

void* BmrNet::reciveAndSend(const void* data, int get_size, int send_size){

	char * buffer = new char[get_size];
	reciveMsg(buffer, get_size);
	sendMsg(data, send_size);
	return buffer;
}

bool BmrNet::sendMsg(const void* data, int size){
	int n;
	n = Sockwrite(this->socketFd, data, size);
	if (n < 0){
		cout<<"ERROR writing to socket"<<endl;
		return false;
	}
	return true;
}

bool BmrNet::reciveMsg(void* buff, int size){
	int n;
	memset(buff, 0,(unsigned long)size);
	n = Sockread(this->socketFd, buff, size);
	if (n < 0){
		cout<<"ERROR reading from socket"<<endl;
		return false;
	}
	printf("Here is the message: %s, %d\n",buff,size);
	return true;
}




