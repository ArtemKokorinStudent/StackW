template <typename T>
class stack
{
public:
	stack() : array_(nullptr), array_size_(0), count_(0) {} /*noexcept*/
	stack(const stack & _stack); /*noexcept*/
	stack& operator=(const stack & _stack); /*basic*/
	size_t count() const; /*noexcept*/
	void push(T const &); /*strong*/
	void pop(); /*noexcept*/
	T& top(); /*strong*/
	~stack(); /*noexcept*/
private:
	T* array_;
	size_t array_size_;
	size_t count_;
	void rereserve(size_t new_size, size_t n_elements_to_copy); /*strong*/
	T* newCopiedArray(const T* source, size_t source_size, size_t destination_size); /*strong*/
};
//T: T(), operator=, 
template<typename T>
stack<T>::stack(const stack & _stack)
	: array_(newCopiedArray(_stack.array_, _stack.count_, _stack.array_size_)),
	array_size_(array_ != nullptr ? _stack.array_size_ : 0),
	count_(array_ != nullptr ? _stack.count_ : 0) {
	;
}

template<typename T>
stack<T>& stack<T>::operator=(const stack & _stack) {
	if (this == &_stack) {
		return *this;
	}
	if (array_size_ < _stack.count_) {
		T* new_array = new T[_stack.array_size_];
		delete[] array_;
		array_ = new_array;
		array_size_ = _stack.array_size_;
	}
	std::copy(_stack.array_, _stack.array_ + _stack.count_, array_);
	count_ = _stack.count_;
	return *this;
}

template<typename T>
size_t stack<T>::count() const
{
	return count_;
}

template<typename T>
void stack<T>::push(T const & new_element)
{
	if (count_ >= array_size_) {
		rereserve((array_size_ * 3) / 2 + 1, count_); //can throw
	}
	try {
		array_[count_] = new_element;
	}
	catch (...) {
		count_--;
	}
	count_++;
}

template<typename T>
void stack<T>::pop()
{
	if (count_ != 0) {
		count_--;
	}
}

template<typename T>
T& stack<T>::top()
{
	if (count_ == 0) {
		throw ("count_ == 0");
	}
	return array_[count_ - 1];
}

template<typename T>
stack<T>::~stack()
{
	delete[] array_;
}

template<typename T>
void stack<T>::rereserve(size_t new_size, size_t n_elements_to_copy) {
	T* new_array = newCopiedArray(array_, count_, new_size);
	if (new_array == nullptr) throw(1);
	delete[] array_;
	array_ = new_array;
	array_size_ = new_size;
}

template<typename T>
T* stack<T>::newCopiedArray(const T* source, size_t source_count, size_t destination_size)
{
	T* new_array = nullptr;
	try {
		new_array = new T[destination_size];
		std::copy(source, source + source_count, new_array); //Throws if an element assignment throws
	}
	catch (...) {
		delete[] new_array;
		new_array = nullptr;
	}
	return new_array;
}
