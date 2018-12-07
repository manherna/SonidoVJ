#pragma once
#include <mutex>
#include <queue>


template<typename Data>
class concurrent_queue
{
private:
	std::queue<Data> the_queue;
	mutable std::mutex the_mutex;
public:
	void push(const Data& data)
	{
		std::mutex::lock(&the_mutex);
		the_queue.push(data);
		std::mutex::unlock(&the_mutex);
	}

	bool empty() const
	{
		bool isEmpty;
		std::mutex::lock(&the_mutex);
		isEmpty =  the_queue.empty();
		std::mutex::unlock(&the_mutex);
		return isEmpty;
	}

	Data& front()
	{
		std::mutex::lock(&the_mutex);
		Data aux = the_queue.front();
		std::mutex::unlock(&the_mutex);
	}

	Data const& front() const
	{
		std::mutex::lock(&the_mutex);
		Data aux = the_queue.front();
		std::mutex::unlock(&the_mutex);
	}

	void pop()
	{
		std::mutex::lock(&the_mutex);
		the_queue.pop();
		std::mutex::unlock(&the_mutex);
	}
};
