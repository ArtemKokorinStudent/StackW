#include <memory>

class bitset
{
public:
	explicit bitset(size_t size); /*strong*/

	bitset(bitset const & other) = delete;
	auto operator=(bitset const & other) -> bitset & = delete;

	bitset(bitset && other) = delete;
	auto operator=(bitset && other)->bitset & = delete;

	void set(size_t index); /*strong*/
	void reset(size_t index); /*strong*/
	bool test(size_t index) const; /*strong*/

	auto size() -> size_t { return size_; } /*noexcept*/
	auto counter() -> size_t { 
		return counter_;
	} /*noexcept*/
private:
	std::unique_ptr<bool[]> ptr_;
	size_t size_;
	size_t counter_;
};

bitset::bitset(size_t size) : 
	ptr_(std::make_unique<bool[]>(size)), 
	size_(size), 
	counter_(0) { /*strong*/
	for (size_t i = 0; i < size; ++i) {
		ptr_[i] = false;
	}
}

void bitset::set(size_t index) { /*strong*/
	if (index < size_) {
		ptr_[index] = true;
		++counter_;
	}
	else {
		throw("index >= size_");
	}
}

void bitset::reset(size_t index) { /*strong*/
	if (index < size_) {
		ptr_[index] = false; 
		--counter_; 
	}
	else {
		throw("index >= size_");
	}
}

bool bitset::test(size_t index) const { /*strong*/
	if (index < size_) {
		return ptr_[index];
	}
	else {
		throw("index >= size_");
	}
}
