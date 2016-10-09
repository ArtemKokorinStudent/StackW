#include <utility>
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
allocator<T>::allocator(size_t size):
	ptr_(size == 0 ? nullptr : static_cast<T*>(operator new(size * sizeof(T)))),
		size_(size), count_(0) /*strong*/
{
	;
}

template<typename T>
allocator<T>::~allocator() /*noexcept*/
{
	for (size_t i = 0; i < count_; i++) {
		ptr_[i].~T();
	}
	
	::operator delete[](ptr_);
}

template<typename T>
auto allocator<T>::swap(allocator & other) -> void /*noexcept*/
{
	std::swap(ptr_, other.ptr_);
	std::swap(size_, other.size_);
	std::swap(count_, other.count_);
}
