template<typename T>
T* newCopiedArray(const T* source, size_t source_count, size_t destination_size) /*strong*/
{
	T* new_array = nullptr;
	try {
		new_array = new T[destination_size];
		std::copy(source, source + source_count, new_array); //Throws if an element assignment throws
	}
	catch (...) {
		delete[] new_array;
		throw;
	}
	return new_array;
}

template <typename T>
class allocator
{
protected:
	allocator(size_t size = 0);
	~allocator();
	auto swap(allocator & other) -> void;

	allocator(allocator const &) = delete;
	auto operator =(allocator const &)->allocator & = delete;

	T * ptr_;
	size_t size_;
	size_t count_;
};

template<typename T>
allocator<T>::allocator(size_t size)
{
	ptr_ = new T[size];
}

template<typename T>
allocator<T>::~allocator()
{
	delete[] ptr_;
}

template<typename T>
auto allocator<T>::swap(allocator & other) -> void
{
	if (count_ > other.size_ || other.count_ > size_) {

	}
}
