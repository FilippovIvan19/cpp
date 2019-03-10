#include <stdio.h>
//#include <stdlib.h>


#define MEOW ;


#ifdef MEOW
    #include <iostream>
    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";


    #define assert( check )\
        if(!(check))\
        {\
            printf("OOPS: %s in %s, line %d, function %s\n", #check, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
            abort();\
        }
#else
    #define $(x) ;
    #define assert( check ) ;
#endif


typedef int vec_t;
const vec_t BIRD = (vec_t)-1245666;


class Vector
{
    private:
        int capacity_;
        int size_;
        vec_t *data_;
    public:
        vec_t& operator[](int index);
        Vector(int cap);
        Vector(const Vector &v);
       ~Vector();
        void resize(int cap);
        int capacity();
        int size();
        vec_t& front();
        vec_t& back();
        void push_back(vec_t val);
        vec_t pop_back();
        bool ok();
        const Vector& operator=(Vector v);
        void swap(Vector &v);
};



int main()
{
    Vector v1(6);
    v1.push_back(14);
    v1.push_back(214);
    v1.push_back(4);
    v1.push_back(1);

    Vector v2(v1);
    Vector v3(2);
    v3 = v1;

    for (int i = 0; i < v2.capacity(); i++)
        printf("%d ", v2[i]);
    printf("\n");
    for (int i = 0; i < v3.capacity(); i++)
        printf("%d ", v3[i]);
    printf("\n\n");

    //printf("%d %d %p\n", v1.capacity(), v1.size(), v1.data_);
    //printf("%d %d %p\n", v2.capacity(), v2.size(), v2.data_);
    //printf("%d %d %p\n", v3.capacity(), v3.size(), v3.data_);

    return 0;
}

//---------------------------------------------------------------------
//! vector constructor
//!
//! @param in [cap]          size of new vector
//---------------------------------------------------------------------
Vector::Vector(int cap):
    capacity_(cap),
    size_(0),
    data_ (new vec_t [cap + 2] {} + 1) //for canary
    {
        this->data_[cap] = BIRD;
        this->data_[-1]  = BIRD;
        assert(ok());
    }

//---------------------------------------------------------------------
//! vector constructor with deep copy
//!
//! @param in [v]          prototype of creating vector
//---------------------------------------------------------------------
Vector::Vector(const Vector &v):
    capacity_(v.capacity_),
    size_(v.size_),
    data_ ((new vec_t [v.capacity_ + 2] {}) + 1) //for canary
    {
        this->data_[v.capacity_] = BIRD;
        this->data_[-1]           = BIRD;

        for (int i = 0; i < this->capacity_; i++)
            this->data_[i] = v.data_[i];

        assert(ok());
    }

//---------------------------------------------------------------------
//! resizes vector to "cap" size
//!
//! @param in [cap]          size of resizer vector
//---------------------------------------------------------------------
void Vector::resize(int cap)
{
    assert(ok());

    vec_t *new_data = (new vec_t [cap + 2] {}) + 1; //for canary
    for (int i = 0; i < this->capacity_ && i < cap; i++)
        new_data[i] = this->data_[i];

    this->capacity_ = cap;
    delete [] (this->data_ - 1); //for canary
    this->data_ = new_data;
    if (this->size_ > this->capacity_)
        this->size_ = this->capacity_;

    this->data_[-1]  = BIRD;
    this->data_[cap] = BIRD;

    assert(ok());
}

//---------------------------------------------------------------------
//! pushes new element into vector
//!
//! @param in [val]          new element
//---------------------------------------------------------------------
void Vector::push_back(vec_t val)
{
    assert(ok());

    if (this->size_ == this->capacity_)
        this->resize(this->capacity_ * 2);

    this->data_[this->size_] = val;
    this->size_++;

    assert(ok());
}

//---------------------------------------------------------------------
//! pops element from vector
//!
//! @returns poped element from vector
//---------------------------------------------------------------------
vec_t Vector::pop_back()
{
    assert(ok());

    if (this->size_ * 4 <= this->capacity_)
        this->resize(this->capacity_ / 2);

    this->size_--;
    return this->data_[this->size_];

    assert(ok());
}

//---------------------------------------------------------------------
//! vector destructor
//---------------------------------------------------------------------
Vector::~Vector()
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
vec_t& Vector::operator[](int index)
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
const Vector& Vector::operator=(Vector v)
{
    assert(ok());

    swap(v);

    assert(ok());
    return *this;
}

//---------------------------------------------------------------------
//! @returns link to last element from vector
//---------------------------------------------------------------------
vec_t& Vector::back()
{
    assert(ok());
    return this->data_[this->size_ - 1];
    assert(ok());
}

//---------------------------------------------------------------------
//! sawps two vectors
//---------------------------------------------------------------------
void Vector::swap(Vector &v)
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
vec_t& Vector::front()
{
    assert(ok());
    return this->data_[0];
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector's capacity
//---------------------------------------------------------------------
int Vector::capacity()
{
    assert(ok());
    return this->capacity_;
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector's size
//---------------------------------------------------------------------
int Vector::size()
{
    assert(ok());
    return this->size_;
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns vector is working right
//---------------------------------------------------------------------
bool Vector::ok()
{
    return (this != nullptr)
        && (size_ <= capacity_)
        && (capacity_ > 0)
        && (data_ != nullptr)
        && (data_[-1]  == BIRD)
        && (data_[capacity_] == BIRD);
}


#undef assert
#undef $
