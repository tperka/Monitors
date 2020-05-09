#include "buffer.hpp"
#include <iostream>

using namespace std;

Buffer::Buffer()
{

	capacity = BUF_SIZE;
	size = 0;
	readByA = readByB = false;
	buffer = new int[capacity];
}

Buffer::~Buffer()
{
	delete[] buffer;
}

void Buffer::insert(int value)
{
	enter();
	//cout << "Producer tries to insert item... " << endl;	
	if(isFull())
	{
		//cout << "Producer is waiting for not full" << endl;
		wait(notFull);
	}

	buffer[size] = value;
	++size;
	cout << "Producer inserted an item: " << value << endl;
	show();

	if(size == 1)
	{
		//cout << "Producer signals that queue is not empty" << endl;
		signal(notEmpty);
	}


	leave();
}

void Buffer::readA()
{
	enter();
//	cout << "A tries to read item... " << endl;	
	if( isEmpty())
	{
		//cout << "A encountered empty queue" << endl;
		leave();
		return;
	}

	if(readByA)
	{	
	//	cout << "A waits for notReadByA"  << endl;
		wait(notReadByA);
	}

	readByA = true;
	std :: cout << "A read an item: " << buffer[0] << endl;

	if(!readByB)
	{
	//	cout << "A signaled read" << endl;
		signal(read);
	}
	leave();

}

void Buffer::readB()
{
	enter();
	//cout << "B tries to read item... " << endl;	
	if( isEmpty())
	{
		//cout << "B encountered empty queue" << endl;
		leave();
		return;
	}

	if(readByB)
	{
		//cout << "B waits for notReadByB"  << endl;
		wait(notReadByB);
	}

	readByB = true;
	std :: cout << "B read an item: " << buffer [0] << endl;

	if(!readByA)
	{
		//cout << "B signaled read" << endl;
		signal(read);
	}

	leave();

}

void Buffer::consume()
{
	enter();
//	cout << "Consumer tries to take item... " << endl;	
	int result;

	if(isEmpty())
	{
		//cout << "Consumer waits for not empty" << endl;
		wait(notEmpty);
	}
	if(!(readByA || readByB))
	{
		//cout << "Consumer waits for read" << endl;
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
	
	if(readByA)	
	{
		readByA = false;
		signal(notReadByA);
	}

	if(readByB)
	{
		readByB = false;
		signal(notReadByB);
	}

	if(size == capacity-1)
	{
		//cout << "Consumer signals that queue is not full" << endl;
		signal(notFull);
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
