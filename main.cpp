#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <boost/random/mersenne_twister.hpp>

#include "buffer.hpp"

#define PRODUCER_MIN 1
#define PRODUCER_MAX 2

#define CONSUMER_MIN 0
#define CONSUMER_MAX 1

#define A_MIN 1
#define A_MAX 2

#define B_MIN 1
#define B_MAX 2


using namespace std;

void *Producer(void *);
void *Consumer(void *);
void *readerA(void *);
void *readerB(void *);

Buffer buffer;	//tworze globalny obiekt klasy bufor, wspoldzielony pomiedzy watki


boost::random::mt19937 gen(time(0));

int main()
{
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

	//dolaczenie watkow, zeby main() czekal na ich zakonczenie
	pthread_join(producer_thread_id, NULL);
	pthread_join(consumer_thread_id, NULL);
	pthread_join(readerA_thread_id, NULL);
	pthread_join(readerB_thread_id, NULL);
	
	return 0;
}

void randomSleep(int max, int min)
{
	usleep(1000000*min + (gen() % 1000000) * (max - min));
}

void *Producer(void*)
{
	int item;
	for(;;)
	{
		//produkuje wartosc
		item = gen() % 100;
		//probuje ja wstawic do bufora
		buffer.insert(item);
		randomSleep(PRODUCER_MIN, PRODUCER_MAX);
	}	
}

void *Consumer(void*)
{
	for(;;)
	{
		buffer.consume();
		randomSleep(CONSUMER_MIN, CONSUMER_MAX);
	}
}

void *readerA(void*)
{
	for(;;)
	{
		buffer.readA();
		randomSleep(A_MIN, A_MAX);
	}
}

void *readerB(void*)
{
	for(;;)
	{
		buffer.readB();
		randomSleep(B_MIN, B_MAX);
	}
}

