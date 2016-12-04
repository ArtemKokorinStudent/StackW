#include "allocator.hpp"
#include <mutex>
#include <vector>

template <typename T>
class stack {
public:
	explicit stack(size_t size = 0);
	stack(stack const & other);
	auto operator=(stack const & other) /*strong*/ -> stack &;

	auto count() const -> size_t { return allocator_.count(); } /*noexcept*/
	bool empty() const { return allocator_.empty(); } /*noexcept*/

	void push(T const & value); /*strong*/
	void pop(); /*strong*/
	auto top()->T &; /*strong*/
	//auto top()->T const &; /*strong*/
private:
	allocator<T> allocator_;
	std::mutex mutex;
};

template<typename T>
stack<T>::stack(size_t size) : allocator_(size) {
	;
}

template<typename T>
stack<T>::stack(stack const & other) : allocator_(other.allocator_) {
	;
}

template<typename T>
auto stack<T>::operator=(stack const & other) -> stack & {
	if (this != &other) {
		mutex.lock();
		(allocator<T>(other.allocator_)).swap(allocator_);
		mutex.unlock();
	}
	return *this;
}

template<typename T>
void stack<T>::push(T const & value) {
	mutex.lock();
	if (allocator_.full()) {
		allocator_.resize();
	}
	allocator_.construct(this->count(), value);
	mutex.unlock();
}

template<typename T>
void stack<T>::pop() {
	mutex.lock();
	if (this->count() > 0) {
		allocator_.destroy(this->count() - 1);
	}
	else {
		mutex.unlock();
		throw("stack is empty");
	}
	mutex.unlock();
}

template<typename T>
auto stack<T>::top() -> T & {
	mutex.lock();
	if (this->count() > 0) {
		mutex.unlock();
		return allocator_.getElement(this->count() - 1);
	}
	else {
		mutex.unlock();
		throw("stack is empty");
	}
}

/*template<typename T>
auto stack<T>::top() -> T const & {
	mutex.lock();
	if (this->count() > 0) {
		mutex.unlock();
		return allocator_.getElement(this->count() - 1);
	}
	else {
		mutex.unlock();
		throw("stack is empty");
	}
}*/
