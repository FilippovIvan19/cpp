#include "vector.h"


class bool_vect_class
{
    public:
        operator bool();
        void operator=(bool bit);
    private:
        friend Vector<bool>;
        int int_index_;
        int bit_num_;
        Vector<bool> *v_;
        bool_vect_class(Vector<bool> *v);
       ~bool_vect_class();
};


template <>
class Vector<bool>
{
    private:
        friend bool_vect_class;
        class bool_vect_class bvc_;
        int capacity_;
        int size_;
        int *data_;
    public:
        bool_vect_class &operator[](int index);
        Vector(int cap = 8);
        Vector(const Vector &v);
       ~Vector();
        void resize(int new_cap);
        int capacity();
        int size();
        bool_vect_class &front();
        bool_vect_class &back();
        void push_back(bool val);
        bool_vect_class pop_back();
        bool ok();
        const Vector<bool> &operator=(Vector v);
        void swap(Vector &v);
};


const int max_bit_num = 8 * sizeof(int) - 1;


//---------------------------------------------------------------------
//! constructor
//!
//! @param in [v]          pointer to vector that this class belongs to
//---------------------------------------------------------------------
bool_vect_class::bool_vect_class(Vector<bool> *v):
    int_index_(-1),
    bit_num_(-1),
    v_(v)
    {}

//---------------------------------------------------------------------
//! destructor
//---------------------------------------------------------------------
bool_vect_class::~bool_vect_class()
{
    int_index_ = -1;
    bit_num_ = -1;
    v_ = nullptr;
}


//---------------------------------------------------------------------
//! needed to returning v[i] in bool vector
//---------------------------------------------------------------------
bool_vect_class::operator bool()
{
    int int_val = this->v_->data_[this->int_index_];
    return int_val & (1 << (max_bit_num - this->bit_num_));
}

//---------------------------------------------------------------------
//! adapts "v[i] = val" for bool vector
//!
//! @param in [bit]          same as val in "v[i] = val"
//---------------------------------------------------------------------
void bool_vect_class::operator= (bool bit)
{
    int int_val = this->v_->data_[this->int_index_];
    int_val = int_val & ~(1 << (max_bit_num - this->bit_num_));
    int_val = int_val | (bit << (max_bit_num - this->bit_num_));
    this->v_->data_[this->int_index_] = int_val;
}



//---------------------------------------------------------------------
//! vector constructor
//!
//! @param in [cap]          size of new vector
//!
//! (adapted for bool vector)
//---------------------------------------------------------------------
Vector<bool>::Vector(int cap):
    capacity_(cap),
    size_(0),
    data_ (new int [cap / sizeof(int) / 8 + 1 + 2] {} + 1), //for canary
    bvc_(this)
    {
        this->data_[cap / sizeof(int) / 8 + 1] = (int)BIRD;
        this->data_[-1] = (int)BIRD;
        assert(ok());
    }

//---------------------------------------------------------------------
//! vector constructor with deep copy
//!
//! @param in [v]          prototype of creating vector
//!
//! (adapted for bool vector)
//---------------------------------------------------------------------
Vector<bool>::Vector(const Vector<bool> &v):
    capacity_(v.capacity_),
    size_(v.size_),
    data_ ((new int [v.capacity_ / sizeof(int) / 8 + 1 + 2] {}) + 1), //for canary
    bvc_(this)
    {
        this->data_[v.capacity_ / sizeof(int) / 8 + 1] = (int)BIRD;
        this->data_[-1] = (int)BIRD;

        int int_cap = this->capacity_ / sizeof(int) / 8 + 1;
        for (int i = 0; i < int_cap; i++)
            this->data_[i] = v.data_[i];

        assert(ok());
    }

//---------------------------------------------------------------------
//! resizes vector to "new_cap" size
//!
//! @param in [new_cap]          size of resizer vector
//!
//! (adapted for bool vector)
//---------------------------------------------------------------------
void Vector<bool>::resize(int new_cap)
{
    assert(ok());
    int int_cap = this->capacity_ / sizeof(int) / 8 + 1;
    int new_int_cap = new_cap / sizeof(int) / 8 + 1;

    int *new_data = (new int [new_int_cap + 2] {}) + 1; //for canary

    for (int i = 0; i < int_cap && i < new_int_cap; i++)
        new_data[i] = this->data_[i];

    this->capacity_ = new_cap;
    delete [] (this->data_ - 1); //for canary
    this->data_ = new_data;
    if (this->size_ > this->capacity_)
        this->size_ = this->capacity_;

    this->data_[-1]  = (int)BIRD;
    this->data_[new_int_cap] = (int)BIRD;

    assert(ok());
}

//---------------------------------------------------------------------
//! @param in [index]          number of element in vector
//!
//! @returns link to element from vector with number "index"
//!
//! (adapted for bool vector)
//---------------------------------------------------------------------
bool_vect_class& Vector<bool>::operator[](int bool_index)
{
    assert(ok());
    this->bvc_.int_index_ = bool_index / sizeof(int) / 8;
    this->bvc_.bit_num_ = bool_index % (sizeof(int) * 8);
    return bvc_;
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector is working right
//!
//! (adapted for bool vector)
//---------------------------------------------------------------------
bool Vector<bool>::ok()
{
    return (this != nullptr)
        && (size_ <= capacity_)
        && (capacity_ > 0)
        && (data_ != nullptr)
        && (data_[-1]  == (int)BIRD)
        && (data_[capacity_ / sizeof(int) / 8 + 1] == (int)BIRD);
}



//---------------------------------------------------------------------
//! vector destructor
//---------------------------------------------------------------------
Vector<bool>::~Vector()
{
    assert(ok());

    delete [] (this->data_ - 1); //for canary
    this->data_ = nullptr;
}

//---------------------------------------------------------------------
//! pushes new element into vector
//!
//! @param in [val]          new element
//---------------------------------------------------------------------
void Vector<bool>::push_back(bool val)
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
bool_vect_class Vector<bool>::pop_back()
{
    assert(ok());

    this->size_--;
    return this->operator[](this->size_);

    assert(ok());
}

//---------------------------------------------------------------------
//! makes the same vector as v (deep copy)
//!
//! @param in [v]          prototype of creating vector
//!
//! @returns link to vector
//---------------------------------------------------------------------
const Vector<bool>& Vector<bool>::operator=(Vector<bool> v)
{
    assert(ok());

    swap(v);

    assert(ok());
    return *this;
}

//---------------------------------------------------------------------
//! @returns link to last element from vector
//---------------------------------------------------------------------
bool_vect_class& Vector<bool>::back()
{
    assert(ok());
    return this->operator[](this->size_ - 1);
    assert(ok());
}

//---------------------------------------------------------------------
//! sawps two vectors
//---------------------------------------------------------------------
void Vector<bool>::swap(Vector<bool> &v)
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
bool_vect_class& Vector<bool>::front()
{
    assert(ok());
    return this->operator[](0);
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector's capacity
//---------------------------------------------------------------------
int Vector<bool>::capacity()
{
    assert(ok());
    return this->capacity_;
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector's size
//---------------------------------------------------------------------
int Vector<bool>::size()
{
    assert(ok());
    return this->size_;
    assert(ok());
}