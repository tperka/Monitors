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
	bool readByA, readByB;
public:
	Buffer();

	~Buffer();

	void insert(int value);

	void readA();

	void readB();

	void consume();

	bool isEmpty();

	int getSize();

	void show();

	bool isFull();

};

#endif // BUFFER_HPP
