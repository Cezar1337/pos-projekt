

//============================================================================
// Name        : objects-pos-projekt.cpp
// Author      : Cezary Kiedrowski, Arkadiusz Dumin
// Version     : alpha lol
// Copyright   : 
// Description : Several objects on a 2d map, sending location info via IP
//============================================================================

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define MOJ_PORT 4040
#define OCZEKUJACE 1
#define ADRES_IP "192.168.2.1"

struct polozenie
{
	int x;
	int y;
};

polozenie stworzenie_struktury()
{
	srand(time(NULL));
	polozenie obiekt;
	obiekt.x=rand()%21;
	obiekt.y=rand()%11;

	cout<<"Strukture utworzono"<<endl;
	cout<<"Polozenie x: "<<obiekt.x<<endl;
	cout<<"Polozenie y: "<<obiekt.y<<endl;

	return obiekt;
}


//serializacja int, musmimy rozszerzyć o wzięcie drugiej wartości int z drugim polozeniem
unsigned char * serialize_int(unsigned char *buffer, int value_x, int value_y)
{
  /* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char. */
  buffer[0] = value_x >> 24; //przesuniecia bitowe
  buffer[1] = value_x >> 16;
  buffer[2] = value_x >> 8;
  buffer[3] = value_x;
  buffer[4] = value_y >> 24;
  buffer[5] = value_y >>16;
  buffer[6] = value_y >>8;
  buffer[7] = value_y;

  return buffer + 8; //przesuniecie wskaznika na buffer[8], czyli pierwszy wolny adres za tablica
}

//serializacja struktury
unsigned char * serialize_temp(unsigned char *buffer,  const polozenie *value)
{
  buffer = serialize_int(buffer, value->x, value->y); //wskaznik na buffer[3]
  return buffer;
}

//wysyłanie, kwestia czy chcemy dzialac na wskazniku, czy po prostu przez wartosc strukture do funkcji dac
int send_temp(int socket, const  polozenie *obiekt)
{
  unsigned char buffer[32], *ptr;

  ptr = serialize_temp(buffer, obiekt); //wskaznik na buffer[4]
  //wersja z send
  return send (socket, buffer, ptr-buffer, 0); //prt-buffer zwróci liczbe
}

void serwer_uruchomienie(){

	int sockfd, new_fd;
	struct sockaddr_in moj_adres;
	struct sockaddr_in klient_adres;
	unsigned int sin_rozmiar;
	int yes = 1;

	if((sockfd = socket(PF_INET, SOCK_STREAM,0))==-1)
	{
		cout<<"Blad socket"<<endl;
		exit(1);
	}
	else
	{
		cout<<"Wywolano socket() powiodlo sie"<<endl;
	}



	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int))== -1){
		cout<<"Setsockopt"<<endl;
		exit(1);
	}
	else
	{
		cout<<"Wywolano setsockopt() powiodlo sie"<<endl;
	}

	moj_adres.sin_family = AF_INET;
	moj_adres.sin_port = htons(MOJ_PORT);
	moj_adres.sin_addr.s_addr = inet_addr(ADRES_IP);
	memset(&(moj_adres.sin_zero), '\0', 8);

	if(bind(sockfd, (struct sockaddr *)&moj_adres, sizeof(struct sockaddr))== -1){
		cout<<"Blad bind"<<endl;
		exit(1);
	}
	else
	{
		cout<<"Wywolano bind() powiodlo sie"<<endl;
	}

	if (listen(sockfd, OCZEKUJACE) == -1){
		cout<<"Blad bind"<<endl;
		exit(1);
	}
	else
	{
		cout<<"Wywolano listen() powiodlo sie"<<endl;
	}

	sin_rozmiar = sizeof(struct sockaddr_in);

	cout<<"Aktualnie dziala do momentu przed polaczeniem"<<endl;


		if((new_fd = accept(sockfd,(struct sockaddr *)&klient_adres, &sin_rozmiar )) == -1)
		{
			cout<<"Blad accept"<<endl;

		}
		else
		{
			cout<<"Polaczono, accept() dziala"<<endl;
		}

		//pytanie czy ten fork cały
		close(sockfd);
	/*	polozenie obiekt = stworzenie_struktury();
		if(send_temp(new_fd,&obiekt) == -1)
		{
			cout<<"Wysylanie blad"<<endl;
		}
	*/
	int i = 1;
	while(1)
	{

		if(send(new_fd, "hello, world!\n", 14, 0) == -1)
		{
			cout<<"Blad wysylania"<<endl;
		}

		else
		{
			cout<<"Wyslano wiadomosc po raz: "<<i<<endl;
			i++;
			sleep(1);
		}
	}

	close(new_fd);

}

void *serwer(void * unused)
{
	cout<<"Uruchomienie watku"<<endl;
	serwer_uruchomienie();
	pthread_exit(NULL);

}

int main () {

	pthread_t watek;
	pthread_create(&watek, NULL, serwer,(void *) NULL);
	pthread_exit(NULL);

}
