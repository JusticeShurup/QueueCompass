#ifndef _MY_QUEUE_H_
#define _MY_QUEUE_H_
#include <iostream>

template <typename T>
class MyQueue
{
public:

	MyQueue()
	{
		_size = 0;
		_head = nullptr;
	}

	void Push(T value)
	{
		if (_size == 0)
		{
			_head = new QueueElement{ nullptr, value };
			_size++;
			return;
		}
		QueueElement* newElem = new QueueElement{ nullptr, value };
		QueueElement* temp = _head;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		temp->next = newElem;
		_size++;
	}

	T Pop()
	{
		if (Empty())
		{
			throw std::exception("Queue is empty");
		}
		QueueElement* next = _head->next;
		T headValue = _head->value;
		delete _head;
		_size--;
		_head = next;
		return headValue;
	}

	const T& First() const
	{
		if (Empty())
		{
			throw std::exception("Queue is empty");
		}
		return _head->value;
	}

	int Find(T value)
	{
		const QueueElement* temp = _head;
		for (int i = 0; i < _size; i++)
		{
			if (temp->value == value)
			{
				return i;
			}
			temp = temp->next;
		}
		return -1;
	}
	
	const int& Count() const
	{
		return _size;
	}

	bool Empty() const
	{
		return _size == 0;
	}

	void PrintAll() const
	{
		const QueueElement* temp = _head;
		for (int i = 0; i < _size; i++)
		{
			std::cout << temp->value << std::endl;
			temp = temp->next;
		}
	}

private:
	struct QueueElement{
		QueueElement * next;
		T value;
	};

	QueueElement* _head;
	int _size;
};

#endif 