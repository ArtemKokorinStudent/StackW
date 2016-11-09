#include "allocator.hpp"

template <typename T>
class stack {
public:
	explicit stack(size_t size = 0);
	auto operator=(stack const & other) /*strong*/ -> stack &;

	auto count() const -> size_t { return allocator_.count(); } /*noexcept*/
	bool empty() const { return allocator_.empty(); } /*noexcept*/

	void push(T const & value); /*strong*/
	void pop(); /*strong*/
	auto top() -> T &; /*strong*/
	auto top() const -> T const &; /*strong*/
private:
	allocator<T> allocator_;
};

template<typename T>
stack<T>::stack(size_t size) : allocator_(size) {
	; 
}

template<typename T>
auto stack<T>::operator=(stack const & other) -> stack & {
	if (this != &other) {
		(allocator<T>(other.allocator_)).swap(allocator_);
	}
	return *this;
}

template<typename T>
void stack<T>::push(T const & value) {
	if (allocator_.full()) {
		allocator_.resize();
	}
	allocator_.construct(this->count(), value);
}

template<typename T>
void stack<T>::pop() {
	if (this->count() > 0) {
		allocator_.destroy(this->count() - 1);
	}
	else {
		throw("stack is empty");
	}
}

template<typename T>
auto stack<T>::top() -> T & {
	if (this->count() > 0) {
		return allocator_.getElement(this->count() - 1);
	}
	else {
		throw("stack is empty");
	}
}

template<typename T>
auto stack<T>::top() const -> T const & {
	if (this->count() > 0) {
		return allocator_.getElement(this->count() - 1);
	}
	else {
		throw("stack is empty");
	}
}
