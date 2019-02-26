#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <iostream>


#define MEOW ;


#ifdef MEOW
    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";
#else
    #define $(x) ;
#endif

typedef int vec_t;


class Vector
{
    private:
        vec_t *data_;
        int capacity_;
    public:
        vec_t& operator[](int index);
        Vector(int cap);
       ~Vector();
};

Vector::Vector(int cap):
    capacity_(cap),
    data_ (new vec_t [cap] {})
    {}

Vector::~Vector()
{
    delete [] data_;
    data_ = nullptr;
}


vec_t& Vector::operator[](int index)
{
    return data_[index];
}


int main()
{

    //printf("\n\n\n");

    Vector v(5);
    v[0] = 14;
    v[3] = -1;
    printf("%d\n\n\n", v[3]);



    return 0;
}
