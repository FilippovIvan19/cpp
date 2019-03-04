#include <stdio.h>


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
#endif


typedef int st_t;

typedef st_t vec_t;
#include "../vector/vector.h"


class Stack
{
    private:
        Vector *v_;

    public:
        Stack(int cap);
       ~Stack();
        void push(st_t val);
        st_t pop();
        bool ok();
        int size();
        int capacity();
};

int main()
{
    Stack st(2);
    st.push(1);
    st.push(14);
    st.push(11);
    st_t a = st.pop();
    st.push(a);
    st.push(a);
    st.push(a);


    printf("%d\n", st.capacity());
    printf("%d\n", st.size());

    while (st.size() > 0)
        printf("%d\n", st.pop());

    printf("%d\n", st.capacity());
    return 0;
}



//---------------------------------------------------------------------
//! stack constructor
//!
//! @param in [cap]          size of new stack
//---------------------------------------------------------------------
Stack::Stack(int cap):
    v_(new Vector(cap))
    {
        assert(ok());
    }

//---------------------------------------------------------------------
//! stack destructor
//---------------------------------------------------------------------
Stack::~Stack()
{
    delete this->v_;
}

//---------------------------------------------------------------------
//! pushes new element into stack
//!
//! @param in [val]          new element
//---------------------------------------------------------------------
void Stack::push(st_t val)
{
    assert(ok());
    this->v_->push_back(val);
    assert(ok());
}

//---------------------------------------------------------------------
//! pops element from stack
//!
//! @returns poped element from stack
//---------------------------------------------------------------------
st_t Stack::pop()
{
    assert(ok());
    return this->v_->pop_back();
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns stack's size
//---------------------------------------------------------------------
int Stack::size()
{
    assert(ok());
    return this->v_->size();
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns stack's capacity
//---------------------------------------------------------------------
int Stack::capacity()
{
    assert(ok());
    return this->v_->capacity();
    assert(ok());
}

//---------------------------------------------------------------------
//! @returns stack is working right
//---------------------------------------------------------------------
bool Stack::ok()
{
    return (this != nullptr) && (this->v_->ok());
}
