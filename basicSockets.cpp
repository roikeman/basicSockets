/*
 * basicSockets.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: froike
 */

#include "basicSockets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>


using namespace std;

BmrNet::BmrNet(char* host, int portno) {
	this->port = portno;
	this->host = host;
	this->is_JustServer = false;
	this->socketFd = NULL;
}

BmrNet::BmrNet(int portno) {
	this->port = portno;
	this->host = "";
	this->is_JustServer = true;
	this->socketFd = NULL;
}

BmrNet::~BmrNet() {
	close(socketFd);
	printf("Closeing!\n");
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
	bzero((char *) &serv_addr, sizeof(serv_addr));

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
	this->socketFd = accept(serverSockFd,
			(struct sockaddr *) &cli_addr,
			&clilen);
	if (this->socketFd < 0){
		cout<<"ERROR on accept"<<endl;
		return false;
	}

	printf("Connected!");

	close(serverSockFd);
	return true;
}

#define bufferSize 256
bool BmrNet::connectNow(){
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int n;

	if (is_JustServer){
		cout<<"ERROR: Never got an host... please use the secend constractor"<<endl;;
		return false;
	}

	fprintf(stderr,"usage %s hostname port\n", host);
	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFd < 0){
		cout<<("ERROR opening socket")<<endl;
		return false;
	}

	server = gethostbyname(host);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		return false;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(socketFd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		cout<<"ERROR connecting"<<endl;
		return false;
	}


	printf("Connected!");

	return true;

}
void* BmrNet::sendAndRecive(const void* data, int size){

	char * buffer = new char[256];
	sendMsg(data,size);
	reciveMsg(buffer,256);
	return buffer;
}

void* BmrNet::reciveAndSend(void* (*fun)(void*,int), int get_size, int send_size){
	char * buffer = new char[get_size];
	reciveMsg(buffer,get_size);
	sendMsg( fun(buffer, send_size) ,send_size );
	return buffer;
}

void* BmrNet::reciveAndSend(const void* data, int size){

	char * buffer = new char[size];
	reciveMsg(buffer,size);
	sendMsg(data,size);
	return buffer;
}

bool BmrNet::sendMsg(const void* data, int size){
	int n;
	n = write(this->socketFd, data, size);
	if (n < 0){
		cout<<"ERROR writing to socket"<<endl;
		return false;
	}
	return true;
}

bool BmrNet::reciveMsg(void* buff, int buffSize){
	int n;
	bzero(buff,(unsigned long)buffSize);
	n = read(this->socketFd,buff,bufferSize-1);
	if (n < 0){
		cout<<"ERROR reading from socket"<<endl;
		return false;
	}
	printf("Here is the message: %s\n",buff);
	return true;
}




