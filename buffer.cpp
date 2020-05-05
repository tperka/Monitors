#include "buffer.hpp"
#include <iostream>


Buffer::Buffer()
: notFull(1), notReadByA(1), notReadByB(1), notEmpty(0), read(0)
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
	if(isFull())
		wait(notFull);

	buffer[size] = value;
	++size;
	std::cout << "Producer inserted an item: " << value << std::endl;
	show();

	if(size == 1)
		signal(notEmpty);

	leave();
}

void Buffer::readA()
{
	enter();
	if( isEmpty())
	{
		leave();//wait(notEmpty);
		return;
	}

	wait(notReadByA);
	signal(read);
	std :: cout << "A read an item: " << buffer[0] << std::endl;
	leave();

}

void Buffer::readB()
{
	enter();
	if( isEmpty())
	{
		leave();
		return;
	}

	wait(notReadByB);
	signal(read);
	std :: cout << "B read an item: " << buffer [0] << std::endl;
	leave();

}

void Buffer::consume()
{
	enter();
	
	int result;

	if(isEmpty())
		wait(notEmpty);

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
		signal(notFull);
	
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
