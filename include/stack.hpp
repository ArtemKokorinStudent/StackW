#include "allocator.hpp"
#include <mutex>
#include <vector>
#include <iostream>

template <typename T>
class stack {
public:
	explicit stack(size_t size = 0);
	auto operator=(stack const & other) /*strong*/ -> stack &;

	auto count() const -> size_t { return allocator_.count(); } /*noexcept*/
	bool empty() const { return allocator_.empty(); } /*noexcept*/

	void push(T const & value); /*strong*/
	void pop(); /*strong*/
	auto top()->T &; /*strong*/
	//auto top()->T const &; /*strong*/
private:
	allocator<T> allocator_;
	std::mutex mutex1;
};

template<typename T>
stack<T>::stack(size_t size) : allocator_(size) {
	;
}

template<typename T>
auto stack<T>::operator=(stack const & other) -> stack & {
	if (this != &other) {
		mutex1.lock();
		(allocator<T>(other.allocator_)).swap(allocator_);
		mutex1.unlock();
	}
	return *this;
}

template<typename T>
void stack<T>::push(T const & value) {
	mutex1.lock();
	if (allocator_.full()) {
		allocator_.resize();
	}
	allocator_.construct(this->count(), value);
	mutex1.unlock();
}

template<typename T>
void stack<T>::pop() {
	mutex1.lock();
	if (this->count() > 0) {
		allocator_.destroy(this->count() - 1);
	}
	else {
		mutex1.unlock();
		throw("stack is empty");
	}
	mutex1.unlock();
}

template<typename T>
auto stack<T>::top() -> T & {
	mutex1.lock();
	if (this->count() > 0) {
		mutex1.unlock();
		return allocator_.getElement(this->count() - 1);
	}
	else {
		mutex1.unlock();
		throw("stack is empty");
	}
}

/*template<typename T>
auto stack<T>::top() -> T const & {
	mutex1.lock();
	if (this->count() > 0) {
		mutex1.unlock();
		return allocator_.getElement(this->count() - 1);
	}
	else {
		mutex1.unlock();
		throw("stack is empty");
	}
}*/
