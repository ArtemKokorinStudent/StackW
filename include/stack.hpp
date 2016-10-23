#include <iostream>
#include <stdexcept>

template <typename T>
auto new_with_copy( T const * src, size_t src_size, size_t dst_size ) -> T *
{
    T * dst = new T [ dst_size ];
    
    try {
        std::copy( src, src + src_size, dst );
    }
    catch (...) {
        delete [] dst;
        throw;
    }
    
    return dst;
}


template <typename T>
class stack {
public:
    stack();
    stack( stack const & );
    ~stack();
    auto operator =( stack const & ) -> stack &;
    
    auto empty() const noexcept -> bool;
    auto count() const noexcept -> size_t;
    auto push( T const & value ) -> void;
    auto pop() -> void;
    auto top() -> T &;
    auto top() const -> T const &;
    
private:
    auto throw_is_empty() -> void;
    auto swap( stack & other ) -> void;
    
    T * ptr_;
    size_t size_;
    size_t count_;
};

template <typename T>
stack<T>::stack() :
    ptr_( nullptr ),
    size_( 0 ),
    count_( 0 )
{
    ;
}

template <typename T>
stack<T>::stack(stack const & other) :
    ptr_( new_with_copy( other.ptr_, other.count_, other.count_) ),
    size_( other.count_ ),
    count_( other.count_ )
{
    ;
}

template <typename T>
auto stack<T>::operator =(stack const & other) -> stack &
{
    if ( this != &other ) {
        this->swap( stack( other ) );
    };
    
    return *this;
}

template <typename T>
stack<T>::~stack<T>()
{
    delete [] ptr_;
}

template <typename T>
auto stack<T>::empty() const noexcept -> bool
{
    return count_ == 0;
}

template <typename T>
auto stack<T>::count() const noexcept -> size_t
{
    return count_;
}

template <typename T>
auto stack<T>::push(const T & value) -> void
{
    if ( count_ == size_ ) {
        size_t size = size_ * 2 + (size_ == 0);
        
        T * ptr = new_with_copy(ptr_, count_, size);
        
        delete [] ptr_;
        ptr_ = ptr;
        size_ = size;
        ptr_ = ptr;
    }
    
    ptr_[count_] = value;
    ++count_;
}

template <typename T>
auto stack<T>::pop() -> void
{
    if ( count_ == 0 ) {
        throw_is_empty();
    }
    
    --count_;
}

template <typename T>
auto stack<T>::top() -> T &
{
    if ( count_ == 0 ) {
        throw_is_empty();
    }
    
    return ptr_[count_ - 1];
}

template <typename T>
auto stack<T>::top() const -> T const &
{
    if ( count_ == 0 ) {
        throw_is_empty();
    }
    
    return ptr_[count_ - 1];
}

template <typename T>
auto stack<T>::throw_is_empty() -> void
{
    throw std::logic_error("stack is empty");
}

template <typename T>
auto stack<T>::swap( stack & other ) -> void
{
    std::swap( ptr_, other.ptr_ );
    std::swap( size_, other.size_ );
    std::swap( count_, other.count_ );
}

