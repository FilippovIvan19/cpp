#include <utility>

const int BIRD = -1245666;


template <typename DataT = int>
class Vector
{
    private:
        int capacity_;
        int size_;
        DataT *data_;
    public:
        DataT &operator[](int index);
        Vector(int cap = 8);
        Vector(const Vector &v);
       ~Vector();
        void resize(int new_cap);
        int capacity();
        int size();
        DataT &front();
        DataT &back();
        void push_back(DataT val);
	    DataT pop_back();
        bool ok();
        const Vector<DataT> &operator=(Vector v);
        void swap(Vector &v);
};


//---------------------------------------------------------------------
//! vector constructor
//!
//! @param in [cap]          size of new vector
//---------------------------------------------------------------------
template <typename DataT>
Vector<DataT>::Vector(int cap):
    capacity_(cap),
    size_(0),
    data_ (new DataT [cap + 2] {} + 1) //for canary
    {
        this->data_[cap] = (DataT)BIRD;
        this->data_[-1]  = (DataT)BIRD;
        assert(ok());
    }

//---------------------------------------------------------------------
//! vector constructor with deep copy
//!
//! @param in [v]          prototype of creating vector
//---------------------------------------------------------------------
template <typename DataT>
Vector<DataT>::Vector(const Vector<DataT> &v):
    capacity_(v.capacity_),
    size_(v.size_),
    data_ ((new DataT [v.capacity_ + 2] {}) + 1) //for canary
    {
        this->data_[v.capacity_] = (DataT)BIRD;
        this->data_[-1]          = (DataT)BIRD;

        for (int i = 0; i < this->capacity_; i++)
            this->data_[i] = v.data_[i];

        assert(ok());
    }

//---------------------------------------------------------------------
//! resizes vector to "new_cap" size
//!
//! @param in [new_ cap]          size of resizer vector
//---------------------------------------------------------------------
template <typename DataT>
void Vector<DataT>::resize(int new_cap)
{
    assert(ok());

    DataT *new_data = (new DataT [new_cap + 2] {}) + 1; //for canary
    for (int i = 0; i < this->capacity_ && i < new_cap; i++)
        new_data[i] = this->data_[i];

    this->capacity_ = new_cap;
    delete [] (this->data_ - 1); //for canary
    this->data_ = new_data;
    if (this->size_ > this->capacity_)
        this->size_ = this->capacity_;

    this->data_[-1]  = (DataT)BIRD;
    this->data_[new_cap] = (DataT)BIRD;

    assert(ok());
}

//---------------------------------------------------------------------
//! pushes new element into vector
//!
//! @param in [val]          new element
//---------------------------------------------------------------------
template <typename DataT>
void Vector<DataT>::push_back(DataT val)
{
    assert(ok());

    if (this->size_ == this->capacity_)
        this->resize(this->capacity_ * 2);

    this->operator[](this->size_) = val;
    this->size_++;

    assert(ok());
}

//---------------------------------------------------------------------
//! pops element from vector
//!
//! @returns poped element from vector
//---------------------------------------------------------------------
template <typename DataT>
DataT Vector<DataT>::pop_back()
{
    assert(ok());

    this->size_--;
    return this->operator[](this->size_);

    assert(ok());
}

//---------------------------------------------------------------------
//! vector destructor
//---------------------------------------------------------------------
template <typename DataT>
Vector<DataT>::~Vector()
{
    assert(ok());

    delete [] (this->data_ - 1); //for canary
    this->data_ = nullptr;
}

//---------------------------------------------------------------------
//! @param in [index]          number of element in vector
//!
//! @returns link to element from vector with number "index"
//---------------------------------------------------------------------
template <typename DataT>
DataT& Vector<DataT>::operator[](int index)
{
    assert(ok());
    return this->data_[index];
    assert(ok());
}

//---------------------------------------------------------------------
//! makes the same vector as v (deep copy)
//!
//! @param in [v]          prototype of creating vector
//!
//! @returns link to vector
//---------------------------------------------------------------------
template <typename DataT>
const Vector<DataT>& Vector<DataT>::operator=(Vector<DataT> v)
{
    assert(ok());

    swap(v);

    assert(ok());
    return *this;
}

//---------------------------------------------------------------------
//! @returns link to last element from vector
//---------------------------------------------------------------------
template <typename DataT>
DataT& Vector<DataT>::back()
{
    assert(ok());
    return this->operator[](this->size_ - 1);
    assert(ok());
}

//---------------------------------------------------------------------
//! sawps two vectors
//---------------------------------------------------------------------
template <typename DataT>
void Vector<DataT>::swap(Vector<DataT> &v)
{
    assert(ok());

    std::swap(this->size_, v.size_);
    std::swap(this->data_, v.data_);
    std::swap(this->capacity_, v.capacity_);

    assert(ok());
}

//---------------------------------------------------------------------
//! @returns link to first element from vector
//---------------------------------------------------------------------
template <typename DataT>
DataT& Vector<DataT>::front()
{
    assert(ok());
    return this->operator[](0);
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector's capacity
//---------------------------------------------------------------------
template <typename DataT>
int Vector<DataT>::capacity()
{
    assert(ok());
    return this->capacity_;
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector's size
//---------------------------------------------------------------------
template <typename DataT>
int Vector<DataT>::size()
{
    assert(ok());
    return this->size_;
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector is working right
//---------------------------------------------------------------------
template <typename DataT>
bool Vector<DataT>::ok()
{
    return (this != nullptr)
        && (size_ <= capacity_)
        && (capacity_ > 0)
        && (data_ != nullptr)
        && (data_[-1]  == (DataT)BIRD)
        && (data_[capacity_] == (DataT)BIRD);
}