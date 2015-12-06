/*
 * MAIN.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: froike
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/timeb.h>

#include "basicSockets.h"
using namespace std;

const int rounds = 5;

void* doNoting(void* data, int size){
 return data;
}


int main(const int argc, char * argv[]){
	char str[] = "test";
	char * buffer;
	
	timeb startt, endt;
	startt.time = 0;
	startt.millitm = 0;
	startt.dstflag = 0;
	startt.timezone = 0;
	cout<<argc<<endl;
	printf("arg:%s %s %s\n", argv[0] , argv[1], argv[2]);
	
	if (argc == 3){
		BmrNet bn = BmrNet(argv[1], atoi(argv[2]));
		bn.connectNow();
		ftime(&startt);

		for (int i = 0; i < rounds; ++i) {
			cout<<"first"<<endl;
			buffer = (char*)bn.sendAndRecive(str, sizeof(str), sizeof(str));
			cout<<"Here is the message: "<<buffer<<" "<<sizeof(str)<<endl;
			cout<<"sec"<<endl;
			buffer = (char*)bn.sendAndRecive(&str, sizeof(str), sizeof(str));
			cout<<"Here is the message: "<<buffer<<" "<<sizeof(str)<<endl;
			cout<<"done"<<endl;
		}


	}else if(argc == 2){
		BmrNet bn = BmrNet(atoi(argv[1]));
		bn.listenNow();
		ftime(&startt);
		for (int i = 0; i < rounds; ++i) {
			cout<<"first"<<endl;
			buffer = (char*)bn.reciveAndSend(str, sizeof(str), sizeof(str));
			cout<<"Here is the message: "<<buffer<<" "<<sizeof(str)<<endl;
			cout<<"sec"<<endl;
			buffer = (char*)bn.reciveAndSend(&doNoting,sizeof(str),sizeof(str));
			cout<<"Here is the message: "<<buffer<<" "<<sizeof(str)<<endl;
			cout<<"done"<<endl;
		}

	}

ftime(&endt);
cout<< "total time:"<<((endt.time - startt.time)*1000.0 +(endt.millitm - startt.millitm))<<" millis"<<endl ;
cout<< "avrg time:"<<((endt.time - startt.time)*1000.0 + (endt.millitm - startt.millitm))/rounds<<" millis"<<endl ;
system("PAUSE");
}



