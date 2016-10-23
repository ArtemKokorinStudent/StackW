#include "allocator.hpp"

template <typename T>
class stack : private allocator<T>
{
public:
	stack(size_t size = 0); /*noexcept*/
	stack(const stack & _stack); /*strong*/
	stack& operator=(const stack & _stack); /*strong*/
	size_t count() const; /*noexcept*/
	void push(T const &); /*strong*/
	const T & top() const; /*strong*/
	void pop(); /*strong*/
	bool empty() { return this->count_ == 0; } /*noexcept*/
	~stack(); /*noexcept*/
};

template<typename T>
stack<T>::stack(size_t size) : allocator<T>(size) {
	;
}

template<typename T>
stack<T>::stack(const stack & _stack) /*strong*/
{
	this->ptr_ = operatorNewCopiedArray(_stack.ptr_, _stack.count_, _stack.size_);
	this->size_ = _stack.size_;
	this->count_ = _stack.count_;
}

template<typename T>
stack<T> & stack<T>::operator=(const stack & _stack) /*strong*/
{
	if (this != &_stack) {
		stack(_stack).swap(*this);
		/*T* midterm = newCopiedArray(_stack.ptr_, _stack.count_, _stack.size_);
		delete[] this->ptr_;
		this->ptr_ = midterm;
		this->count_ = _stack.count_;
		this->size_ = _stack.size_;*/
	}
	return *this;
}

template<typename T>
size_t stack<T>::count() const /*noexcept*/
{
	return this->count_;
}

template <typename T>
void stack<T>::push(const T & value) /*strong*/
{
	if (this->count_ == this->size_) {
		size_t array_size = (this->size_ * 3) / 2 + 1;

		stack temp(array_size);
		while (temp.count() < this->count_) {
			temp.push(this->ptr_[temp.count()]);
		}

		this->swap(temp);
	}
	construct(this->ptr_ + this->count_, value);
	++this->count_;
}

template<typename T>
const T & stack<T>::top() const /*strong*/
{
	if (this->count_ == 0) {
		throw ("top: count_ == 0");
	}
	return this->ptr_[this->count_ - 1];
}

template<typename T>
void stack<T>::pop() /*strong*/
{
	if (this->count_ == 0) {
		throw("pop(): count_ == 0");
	}
	destroy(&(this->ptr_[this->count_ - 1]));
	this->count_--;
}

template<typename T>
stack<T>::~stack() /*noexcept*/
{
	destroy(this->ptr_, this->ptr_ + this->count_);
}
