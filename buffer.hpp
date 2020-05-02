#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "Monitor.hpp"

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
	{
		capacity = BUF_SIZE;
		size = 0;
		buffer = new int[capacity];
		notFull(1);
		notReadByA(1);
		notReadByB(1);
	}

	~Buffer()
	{
		delete[] buffer;
	}

	void push_back(int value)
	{
		enter();
		if(isFull())
			wait(notFull);

		buffer[size] = value;
		++size;

		if(size == 1)
			signal(notEmpty);

		leave();
	}

	void readA_front()
	{
		enter();
		if( isEmpty())
			wait(notEmpty);

		wait(notReadByA);
		signal(read);
		leave();

	}

	void readB_front()
	{
		enter();
		if( isEmpty())
			wait(notEmpty);

		wait(notReadByB);
		signal(read);
		leave();

	}

	int pop_front()
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

		if(size == capacity-1)
			signal(notFull);
		
		signal(notReadByA);
		signal(notReadByB);

		leave();

		return result;
				
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
		cout << "Current queue state: " << endl;
		for(int i = 0; i < size)
		{
			cout << buffer[i] << "\t";
		}
		cout << endl;
	}

	bool isFull()
	{
		return size == capacity;
	}

};

#endif // !1bu BUFFER_HPP
