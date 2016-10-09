#include "allocator.hpp"

template <typename T>
class stack
{
public:
	stack() : array_(nullptr), array_size_(0), count_(0) {} /*noexcept*/
	stack(const stack & _stack); /*strong*/
	stack& operator=(const stack & _stack); /*strong*/
	size_t count() const; /*noexcept*/
	void push(T const &); /*strong*/
	T& top() const; /*strong*/
	void pop(); /*strong*/
	bool empty() { return count_ == 0; } /*noexcept*/
	~stack(); /*noexcept*/
private:
	T* array_;
	size_t array_size_;
	size_t count_;
};
//T: T(), operator=, 
template<typename T>
stack<T>::stack(const stack & _stack) /*strong*/
	: array_(newCopiedArray(_stack.array_, _stack.count_, _stack.array_size_)),
	array_size_(_stack.array_size_),
	count_(_stack.count_) {
	;
}

template<typename T>
stack<T>& stack<T>::operator=(const stack & _stack) /*strong*/
{
	if (this != &_stack) {
		T* midterm = newCopiedArray(_stack.array_, _stack.count_, _stack.array_size_);
		delete[] array_;
		array_ = midterm;
		count_ = _stack.count_;
		array_size_ = _stack.array_size_;
	}
	return *this;
}

template<typename T>
size_t stack<T>::count() const /*noexcept*/
{
	return count_;
}

template<typename T>
void stack<T>::push(T const & new_element) /*strong*/
{
	if (count_ >= array_size_) {
		size_t new_size = (array_size_ * 3) / 2 + 1;
		T* new_array = newCopiedArray(array_, count_, new_size);
		try {
			new_array[count_] = new_element;
		}
		catch (...) {
			delete[] new_array;
			throw;
		}
		delete[] array_;
		array_ = new_array;
		array_size_ = new_size;
	}
	else {
		array_[count_] = new_element;
	}
	count_++;
}

template<typename T>
T& stack<T>::top() const /*strong*/
{
	if (count_ == 0) {
		throw ("top: count_ == 0");
	}
	return array_[count_ - 1];
}

template<typename T>
void stack<T>::pop() /*strong*/
{
	if (count_ == 0) {
		throw("pop(): count_ == 0");
	}
	count_--;
}

template<typename T>
stack<T>::~stack() /*noexcept*/
{
	delete[] array_;
}

