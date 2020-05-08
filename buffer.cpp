#include "buffer.hpp"
#include <iostream>


Buffer::Buffer()
: notFull(0), notReadByA(1), notReadByB(1), notEmpty(0), read(0)
{
	capacity = BUF_SIZE;
	size = 0;
	buffer = new int[capacity];
}

Buffer::~Buffer()
{
	delete[] buffer;
}

void Buffer::insert(int value)
{
	enter();
	std::cout << "Producer tries to insert item... " << std::endl;	
	if(isFull())
	{
		std::cout << "Producer is waiting for not full" << std::endl;
		wait(notFull);
	}

	buffer[size] = value;
	++size;
	std::cout << "Producer inserted an item: " << value << std::endl;
	show();

	if(size == 1)
	{
		std::cout << "Producer signals that queue is not empty" << std::endl;
		signal(notEmpty);
	}


	leave();
}

void Buffer::readA()
{
	enter();
	std::cout << "A tries to read item... " << std::endl;	
	if( isEmpty())
	{
		std::cout << "A encountered empty queue" << std::endl;
		leave();//wait(notEmpty);

		return;
	}

	std::cout << "A checks if he hasn't read the item" << std::endl;
	wait(notReadByA);

	signal(read);
	std :: cout << "A read an item: " << buffer[0] << std::endl;
	leave();

}

void Buffer::readB()
{
	enter();
	std::cout << "B tries to read item... " << std::endl;	
	if( isEmpty())
	{
		std::cout << "B encountered empty queue" << std::endl;

		leave();
		return;
	}
	std::cout << "B checks if he hasn't read the item" << std::endl;

	wait(notReadByB);
	signal(read);
	std :: cout << "B read an item: " << buffer [0] << std::endl;
	leave();

}

void Buffer::consume()
{
	enter();
	std::cout << "Consumer tries to take item... " << std::endl;	
	int result;

	if(isEmpty())
	{
		std::cout << "Consumer waits for not empty" << std::endl;
		wait(notEmpty);
	}
	std::cout << "Consumer checks if item was read" << std::endl;
	wait(read);

	result = buffer[0];
	for(int i = 0; i < size - 1; ++i)
	{
		buffer[i] = buffer[i+1];
	}
	--size;
	std::cout << "Consumer took out an item: " << result << std::endl;
	show();
	if(size == capacity-1)
	{
		std::cout << "Consumer signals that queue is not full" << std::endl;
		signal(notFull);
	}
	
	signal(notReadByA);
	signal(notReadByB);

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
	std::cout << "Current queue state: " << std::endl;
	for(int i = 0; i < size; i++)
	{
		std::cout << buffer[i] << "\t";
	}
	std::cout << std::endl;
}

bool Buffer::isFull()
{
	return size == capacity;
}
