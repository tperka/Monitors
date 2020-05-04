#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "monitor.hpp"

#define BUF_SIZE 9

class Buffer : public Monitor
{
	int *buffer;
	int size;
	int capacity;
	Condition notFull, notEmpty;
	Condition read;
	Condition notReadByA, notReadByB;
public:
	Buffer()
	: notFull(1), notReadByA(1), notReadByB(1), notEmpty(0), read(0)
	{
		capacity = BUF_SIZE;
		size = 0;
		buffer = new int[capacity];
	}

	~Buffer()
	{
		delete[] buffer;
	}

	void insert(int value)
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

	void readA()
	{
		enter();
		if( isEmpty())
			wait(notEmpty);

		wait(notReadByA);
		signal(read);
		std :: cout << "A read an item: " << buffer[0] << std::endl;
		leave();

	}

	void readB()
	{
		enter();
		if( isEmpty())
			wait(notEmpty);

		wait(notReadByB);
		signal(read);
		std :: cout << "B read an item: " << buffer [0] << std::endl;
		leave();

	}

	void consume()
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

	bool isEmpty()
	{
		return size == 0;
	}

	int getSize()
	{
		return size;
	}

	void show()
	{
		std::cout << "Current queue state: " << std::endl;
		for(int i = 0; i < size; i++)
		{
			std::cout << buffer[i] << "\t";
		}
		std::cout << std::endl;
	}

	bool isFull()
	{
		return size == capacity;
	}

};

#endif // !1bu BUFFER_HPP
