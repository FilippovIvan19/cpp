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
