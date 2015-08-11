/*
 * MAIN.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: froike
 */
#include <iostream>
#include <stdlib.h>
#include <sys/timeb.h>

#include "basicSockets.h"
using namespace std;

const int rounds = 10000;

void* doNoting(void* data, int size){
 return data;
}


int main(const int argc, char * argv[]){
	char str[] = "test";
	char * buffer;
	cout<<argc<<endl;
	timeb start, end;


	if (argc == 3){
		BmrNet bn = BmrNet(argv[1], atoi(argv[2]));
		bn.connectNow();
		ftime(&start);

		for (int i = 0; i < rounds; ++i) {
			cout<<"first"<<endl;
			buffer = (char*)bn.sendAndRecive(&str,sizeof(str));
			cout<<"sec"<<endl;
			buffer = (char*)bn.sendAndRecive(&str,sizeof(str));
			cout<<"done"<<endl;
		}


	}else if(argc == 2){
		BmrNet bn = BmrNet(atoi(argv[1]));
		bn.listenNow();
		ftime(&start);
		for (int i = 0; i < rounds; ++i) {
			cout<<"first"<<endl;
			buffer = (char*)bn.reciveAndSend(&str,sizeof(str));
			cout<<"sec"<<endl;
			buffer = (char*)bn.reciveAndSend(&doNoting,sizeof(str),sizeof(str));
			cout<<"done"<<endl;
		}

	}

ftime(&end);
cout<< "total time:"<<((end.time - start.time)*1000.0 +(end.millitm - start.millitm))<<" millis"<<endl ;
cout<< "avrg time:"<<((end.time - start.time)*1000.0 + (end.millitm - start.millitm))/rounds<<" millis"<<endl ;

}



