#include "buffer.hpp"
#include <iostream>

using namespace std;

Buffer::Buffer()
{

	capacity = BUF_SIZE;
	size = 0;
	isReadByA = isReadByB = false;
	buffer = new int[capacity];
}

Buffer::~Buffer()
{
	delete[] buffer;
}

void Buffer::insert(int value)
{
	enter();
	cout << "Producer tries to insert item... " << endl;	
	if(isFull())
	{
		cout << "Producer is waiting for not full" << endl;
		wait(notFull);
	}

	buffer[size] = value;
	++size;
	cout << "Producer inserted an item: " << value << endl;
	show();

	if(size == 1)
	{
		cout << "Producer signals that queue is not empty" << endl;
		signal(notEmpty);
	}

	leave();
}

void Buffer::readA()
{
	enter();
	cout << "A tries to read item... " << endl;	

	if(isReadByA)
	{	
		cout << "A waits for notReadByA"  << endl;
		wait(notReadByA);
	}
	if( isEmpty())
	{
		cout << "A encountered empty queue" << endl;
		wait(notEmpty);
	}


	isReadByA = true;
	cout << "A read an item: " << buffer[0] << endl;

	if(notEmpty.getWaitingCount())
	{
		cout << "A signaled notEmpty" << endl;
		signal(notEmpty);
	}

	if(read.getWaitingCount())
	{
		cout << "A signaled read" << endl;
		signal(read);
	}

	leave();

}

void Buffer::readB()
{
	enter();
	cout << "B tries to read item... " << endl;	

	if(isReadByB)
	{
		cout << "B waits for notReadByB"  << endl;
		wait(notReadByB);
	}

	if( isEmpty())
	{
		cout << "B encountered empty queue" << endl;
		wait(notEmpty);
	}
	
	isReadByB = true;
	cout << "B read an item: " << buffer [0] << endl;

	if(notEmpty.getWaitingCount())
	{
		cout << "B signaled notEmpty" << endl;
		signal(notEmpty);
	}

	if(read.getWaitingCount())
	{
		cout << "B signaled read" << endl;
		signal(read);
	}


	leave();

}

void Buffer::consume()
{
	enter();
	cout << "Consumer tries to take item... " << endl;	
	int result;


	if(!(isReadByA || isReadByB))
	{
		cout << "Consumer waits for read" << endl;
		wait(read);
	}


	result = buffer[0];
	for(int i = 0; i < size - 1; ++i)
	{
		buffer[i] = buffer[i+1];
	}
	--size;
	cout << "Consumer took out an item: " << result << endl;
	show();
	

	

	if(size == capacity-1)
	{
		cout << "Consumer signals that queue is not full" << endl;
		signal(notFull);
	}

	if(isReadByA)	
	{
		isReadByA = false;
		signal(notReadByA);
	}

	if(isReadByB)
	{
		isReadByB = false;
		signal(notReadByB);
	}

	leave();
}

bool Buffer::isEmpty()
{
	return size == 0;
}

int Buffer::getSize()
{
	return size;
}

void Buffer::show()
{
	cout << "Current queue state: " << endl;
	for(int i = 0; i < size; i++)
	{
		cout << buffer[i] << "\t";
	}
	cout << endl;
}

bool Buffer::isFull()
{
	return size == capacity;
}
