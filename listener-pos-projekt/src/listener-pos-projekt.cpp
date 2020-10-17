//============================================================================
// Name        : listener-pos-projekt.cpp
// Author      : Cezary Kiedrowski, Arkadiusz Dumin
// Version     :
// Copyright   : Your copyright notice
// Description : Klientos
//============================================================================


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

#define DOCELOWY_IP "192.168.2.1" //adres serwera
#define PORT 4040 // port do laczenia sie
#define MAXDATA 100

void klient_uruchomienie()
{
	int sockfd, numbytes;
		struct sockaddr_in serwer_adres;
		char buf[MAXDATA];

		if((sockfd = socket(PF_INET, SOCK_STREAM, 0))== -1)
		{
			cout<<"Blad socket"<<endl;
			exit(1);
		}
		else
		{
			cout<<"Powodzenie socket() dziala"<<endl;
		}

		serwer_adres.sin_family = AF_INET;
		serwer_adres.sin_port=htons(PORT);
		serwer_adres.sin_addr.s_addr=inet_addr(DOCELOWY_IP);
		memset(&(serwer_adres.sin_zero), '\0', 8);

		if(connect(sockfd, (struct sockaddr *) &serwer_adres, sizeof(struct sockaddr))== -1)
		{
			cout<<"Blad polaczenie"<<endl;
			exit(1);
		}
		else
		{
			cout<<"Polaczenie dziala"<<endl;
		}

		while(1)
		{
			if((numbytes = recv(sockfd, buf, MAXDATA -1,0))== -1)
			{
				cout<<"Blad recv()"<<endl;
			}

			if(numbytes == 0)
			{
				break;
			}
			else
			{
				buf[numbytes] = '\0';
				cout<<"Otrzymano: "<<buf<<endl;
			}
		}

		close(sockfd);

}

void *klient(void * unused)
{
	klient_uruchomienie();
	pthread_exit(NULL);
}
int main(){

	pthread_t watek;
	pthread_create(&watek, NULL, klient, (void *) NULL);
	pthread_exit(NULL);

}
