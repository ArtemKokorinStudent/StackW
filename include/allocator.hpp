#include <utility>
#include "bitset.hpp"

template<typename T>
T* operatorNewCopiedArray(const T * source, size_t source_count, size_t destination_size) /*strong*/
{
	T* new_array = nullptr;
	size_t n_placed_elements = 0;
	try {
		new_array = static_cast<T*>(operator new(destination_size * sizeof(T)));
		for (size_t i = 0; i < source_count; ++i) {
			construct(&new_array[i], source[i]);
			++n_placed_elements;
		}
	}
	catch (...) {
		destroy(new_array, new_array + n_placed_elements);
		operator delete(new_array);
		throw;
	}
	return new_array;
}

template <typename T>
class allocator
{
public:
	explicit allocator(size_t size = 0); /*strong*/
	allocator(allocator const & other); /*strong*/
	auto operator=(allocator const & other) -> allocator & = delete;
	~allocator();

	auto resize() -> void; /*strong*/

	auto construct(size_t index, T const & value) -> void; /*strong*/
	auto destroy(size_t index) -> void; /*noexcept*/

	auto get() -> T * { return ptr_; } /*noexcept*/
	auto get() const -> T const * { return ptr_; } /*noexcept*/
	auto getElement(size_t index) -> T & { return ptr_[index]; } /*noexcept*/
	auto getElement(size_t index) const -> T const & { return ptr_[index]; } /*noexcept*/

	auto count() const -> size_t { return map_->counter(); } /*noexcept*/
	bool full() const { return map_->counter() == size_; } /*noexcept*/
	bool empty() const { return map_->counter() == 0; } /*noexcept*/
	void swap(allocator & other); /*noexcept*/
private:
	//auto destroy(T * first, T * last) -> void; /*noexcept*/
	
	T * ptr_;
	size_t size_;
	std::unique_ptr<bitset> map_;
};

template<typename T>
allocator<T>::allocator(size_t size) 
	: ptr_(static_cast<T*>(operator new(size * sizeof(T)))),
	size_(size),
	map_(std::make_unique<bitset>(size)) 
{
	;
}

template<typename T>
allocator<T>::allocator(allocator const & other)
	: allocator<T>(other.size_)
{
	try {
		for (size_t i = 0; i < size_; ++i) {
			construct(i, other.ptr_[i]);
		}
	}
	catch (...) {
		this->~allocator();
		throw;
	}
}

template<typename T>
allocator<T>::~allocator() {
	for (size_t i = 0; i < size_; ++i) {
		if (map_->test(i)) {
			this->destroy(i);
		}
	}
	operator delete(ptr_);
}

template<typename T>
void allocator<T>::resize() {
	allocator<T> temp((size_ * 3) / 2 + 1);
	for (size_t i = 0; i < size_; ++i) {
		if (map_->test(i)) {
			temp.construct(i, ptr_[i]);
		}
	}
	this->swap(temp);
}

template<typename T>
void allocator<T>::construct(size_t index, T const & value) {
	if (index < size_) {
		new (&(ptr_[index])) T(value);
		map_->set(index);
	}
	else {
		throw("index >= size_");
	}
}

template<typename T>
void allocator<T>::destroy(size_t index) {
	if (index < size_) {
		if (map_->test(index)) {
			ptr_[index].~T();
			map_->reset(index);
		}
		else {
			throw ("memory is occupied");
		}
	}
	else {
		throw ("index >= size_");
	}
}

/*template<typename T>
auto allocator<T>::destroy(T * first, T * last)->void {
	if (first >= ptr_&&last <= ptr_ + this->count())
		for (; first != last; ++first) {
			destroy(&*first);
		}
}*/

template<typename T>
void allocator<T>::swap(allocator & other) {
	std::swap(ptr_, other.ptr_);
	std::swap(size_, other.size_);
	std::swap(map_, other.map_);
}
