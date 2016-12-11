#include "allocator.hpp"
#include <mutex>
#include <vector>
#include <memory>

template <typename T>
class stack {
public:
	explicit stack(size_t size = 0);
	stack(stack const & other);
	auto operator=(stack const & other) /*strong*/ -> stack &;

	auto count() const -> size_t { return allocator_.count(); } /*noexcept*/
	bool empty() const { return allocator_.empty(); } /*noexcept*/

	void push(T const & value); /*strong*/
	std::shared_ptr<T> pop(); /*strong*/
private:
	allocator<T> allocator_;
	std::mutex mutex;
};

template<typename T>
stack<T>::stack(size_t size) : allocator_(size) {
	;
}

template<typename T>
stack<T>::stack(stack const & other) {
	mutex.lock();
	allocator_ = other.allocator_;
	mutex.unlock();
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
std::shared_ptr<T> stack<T>::pop() {
	mutex.lock();
	if (allocator_.count() == 0) {
		return nullptr;
	}
	std::shared_ptr<T> top(std::make_shared<T>(
		std::move(allocator_.getElement(allocator_.count() - 1)
			)));
	allocator_.destroy(allocator_.count() - 1);
	mutex.unlock();
	return top;
}
