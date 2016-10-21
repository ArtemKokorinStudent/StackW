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
	const T& top() const; /*strong*/ // метод должен возвращать константную ссылку
	void pop(); /*strong*/
	bool empty() { return this->count_ == 0; } /*noexcept*/
	~stack(); /*noexcept*/
};

template<typename T>
stack<T>::stack(size_t size = 0) : allocator(size) {
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
stack<T>& stack<T>::operator=(const stack & _stack) /*strong*/  //не вызывается очистка памяти ptr_
{
	if (this != &_stack) {
		stack(_stack).swap(*this);
		/*T* midterm = newCopiedArray(_stack.ptr_, _stack.count_, _stack.size_); // убрать комментарии 
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

template<typename T>
void stack<T>::push(T const & new_element) /*strong*/
{
	if (this->count_ >= this->size_) {  //  чисто теоретически count_ может быть  больше чем 1,5 +1 раза больше чем size_ => выделения памяти в след строчки не хватит
		size_t new_size = (this->size_ * 3) / 2 + 1;
		T* new_array = operatorNewCopiedArray(this->ptr_, this->count_, new_size);
		try {
			construct(&new_array[this->count_], new_element);
		}
		catch (...) {
			destroy(new_array, new_array + this->count_);
			::operator delete(new_array);
			throw;
		}
		destroy(this->ptr_, this->ptr_ + this->count_);
		::operator delete(this->ptr_);
		this->ptr_ = new_array;
		this->size_ = new_size;
	}
	else {								
		construct(&(this->ptr_[this->count_]), new_element);		
	}
	this->count_++;
}

template<typename T>
const T& stack<T>::top() const /*strong*/ //I don't understand first const   - конст метод джолжен возвращать конст ссылку
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
	// необходимо очистить память
	this->count_--;
}

template<typename T>
stack<T>::~stack() /*noexcept*/
{
	destroy(this->ptr_, this->ptr_ + this->count_);
}

