#include <iostream>
#include <pthread.h>
#include <boost/random/mersenne_twister.hpp>

#include "buffer.hpp"

using namespace std;

void *Producer(void *);
void *Consumer(void *);
void *readerA(void *);
void *readerB(void *);

Buffer buffer;	//tworze globalny obiekt klasy bufor, wspoldzielony pomiedzy watki


boost::random::mt19937 gen(time(0));

void handleCtrlC(int)
{
	system("ipcrm -a");
	cout << endl << "Everything has been cleared, exiting program..." << endl;
	exit(EXIT_SUCCESS);
}


int main()
{
	//przechwytywanie Ctrl-C
	signal(SIGINT, handleCtrlC);
	pthread_t producer_thread_id, consumer_thread_id, readerA_thread_id, readerB_thread_id;
	//rozpoczecie pracy watkow
	if(pthread_create(&producer_thread_id, NULL, Producer, NULL) != 0
	|| pthread_create(&consumer_thread_id, NULL, Consumer, NULL) != 0
	|| pthread_create(&readerA_thread_id, NULL, readerA, NULL) != 0
	|| pthread_create(&readerB_thread_id, NULL, readerB, NULL) != 0)
	{
		cerr << "Error: could not create thread."<<endl;
		exit(EXIT_FAILURE);
	}

	pthread_join(producer_thread_id, NULL);
	pthread_join(consumer_thread_id, NULL);
	pthread_join(readerA_thread_id, NULL);
	pthread_join(readerB_thread_id, NULL);
	
	return 0;
}

randomSleep(int max, int min)
{
	//usleep(1000000*min + (gen() % 1000000) * (max - min));
}

void *producer(void*)
{
	int item;
	for(;;)
	{
		//produkuje wartosc
		item = gen() % 100;
		//probuje ja wstawic do bufora
		buffer.insert(item);
	}	

}

void *Consumer(void*)
{
	for(;;)
	{
		buffer.consume();
	}
}

void *readerA(void*)
{
	for(;;)
	{
		buffer.readA();
	}
}

void *readerB(void*)
{
	for(;;)
	{
		buffer.readB();
	}
}

