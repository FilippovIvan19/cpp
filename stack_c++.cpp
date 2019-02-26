/// @file stack.cpp
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <iostream>


//#define MEOW ;


#ifdef MEOW
    #define ST_ASSERT(x)\
        if (!StackOk(x))\
        {\
            //StackDump(x, "assertstat.txt", 0);\
            assert(0);\
        }


    #define assert( check )\
        if(!(check))\
        {\
            printf("OOPS: %s in %s, line %d, function %s\n", #check, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
            abort -1;\
        }

    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";
#else
    #define $(x) ;
    #define ST_ASSERT(x) ;
#endif

#typedef st_t int
#define BY_DESIGN(x) x//it's a canary


const int LcAll = 0;
const st_t BIRD = -1245666;
const st_t POISON = -66613666;
const int PRINT_CON = 1;
const int NPRINT_CON = 0;


class Stack
{
    private:
        st_t* data;
        int size;
        int capacity;
        Change_Cap(int coeff);

    public:
        Stack();
       ~Stack();
        void Push(st_t val);
        st_t Pop();
};

Stack::Stack():
    data (new st_t [10] + 1),
    capacity (8),
    size (0),
    {
        BY_DESIGN(this->data[-1]             = BIRD);
        BY_DESIGN(this->data[this->capacity] = BIRD);

        ST_ASSERT(this);
    }


Stack::~Stack()
{
    ST_ASSERT(this);

    for (int i = -1; i <= this->capacity; i++)
        this->data[i] = POISON;

    this->size = -1;
    this->capacity = -1;

    delete [] this->data - 1;
    this->data = (st_t*)-1;
    //delete st;//////////////////////////////////////
}


void Stack::Push(st_t x)
{
    ST_ASSERT(this);

    if (this->size == this->capacity)
        Change_Cap(2);

    this->size++;
    this->data[this->size - 1] = x;

    ST_ASSERT(this);
}


st_t Stack::Pop()
{
    ST_ASSERT(this);
    this->size--;
    ST_ASSERT(this);

    st_t x = this->data[this->size];
    if ((this->size * 4 < this->capacity) && (this->capacity > 3))
        Change_Cap(-2);

    ST_ASSERT(this);
    return x;
}

void Stack::Change_Cap(int coeff)
{
    ST_ASSERT(st);
    assert(coeff != 0 && coeff != 1 && coeff != -1);

    if (coeff > 0)
    {
        BY_DESIGN(this->data = (st_t*)realloc(this->data - 1, (this->capacity * coeff + 2) * sizeof(st_t)) + 1);////////////////////////////////////
        this->capacity *= coeff;
    }
    else
    {
        BY_DESIGN(this->data = (st_t*)realloc(this->data - 1, (this->capacity / -coeff + 2) * sizeof(st_t)) + 1);///////////////////////////////////////
        this->capacity /= -coeff;
    }

    BY_DESIGN(this->data[this->capacity] = BIRD);
    ST_ASSERT(st);
}



bool Stack::Ok()
{
    return (this != NULL)
        && (this->data != NULL) && (this->data != (st_t*)-1)
        && (this->capacity > 0)
        && (0 <= this->size) && (this->size <= this->capacity)
        BY_DESIGN(&& (this->data[this->capacity] == BIRD) && (this->data[-1] == BIRD));

}

//---------------------------------------------------------------------
//! check if stack is available to use
//!
//! @param in [st]          pointer to stack
//!
//! @returns bool value is stack ok or not
//---------------------------------------------------------------------
bool StackOk(const Stack *st);


//---------------------------------------------------------------------
//! prints stack information
//!
//! @param in [st]                  pointer to stack
//! @param in [out_file_name]       name of file for printing info
//! @param in [pr]                  1/0 if needs or not to print in stdout too
//!
//! @returns 0
//---------------------------------------------------------------------
int StackDump(const Stack *st, const char *out_file_name, int pr);

//---------------------------------------------------------------------
//! prints stack information
//!
//! @param in [st]                  pointer to stack
//! @param in [out_file_name]       name of file for printing info
//!
//! @returns 0
//---------------------------------------------------------------------
int half_StackDump(const Stack *st, const char *out_file_name);

//---------------------------------------------------------------------
//! deletes stack
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0
//---------------------------------------------------------------------
int StackDtor(Stack *st);

//---------------------------------------------------------------------
//! adds new element into stack
//!
//! @param in [st]           pointer to stack
//! @param in [x]            new element
//!
//! @returns 0
//---------------------------------------------------------------------
int StackPush(Stack *st, st_t x);

//---------------------------------------------------------------------
//! makes stack twise bigger
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0
//---------------------------------------------------------------------
int StackCap_x2(Stack *st);

//---------------------------------------------------------------------
//! makes stack twise smaller
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0
//---------------------------------------------------------------------
int StackCap_x05(Stack *st);

//---------------------------------------------------------------------
//! returns stack capacity
//!
//! @param in [st]           pointer to stack
//!
//! @returns stack capacity
//---------------------------------------------------------------------
int StackCap(const Stack *st);

//---------------------------------------------------------------------
//! returns stack size
//!
//! @param in [st]           pointer to stack
//!
//! @returns stack size
//---------------------------------------------------------------------
int Stacksize(const Stack *st);

//---------------------------------------------------------------------
//! returns top stack element
//!
//! @param in [st]           pointer to stack
//!
//! @returns top stack element
//---------------------------------------------------------------------
st_t StackPop(Stack *st);

//---------------------------------------------------------------------
//! check if stack works right
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0 if stack works right
//---------------------------------------------------------------------
int UnitTest(Stack *st);

//---------------------------------------------------------------------
//! resizes stack
//! if coeff > 1  make capacity coeff times bigger
//! if coeff < -1 make capacity coeff times smaller
//! if coeff == 0 or 1 or -1 gives assertion error
//!
//! @param in [st]           pointer to stack
//! @param in [coeff]        coefficent of resizing
//!
//! @returns 0
//---------------------------------------------------------------------
int Change_StackCap(Stack *st, int coeff);


int main()
{
    setlocale(LcAll, "");

    Stack st = {};
    UnitTest(&st);

    StackCtor(&st, 10);
    StackDtor(&st);

    //assert(0 == 1);

    return 0;
}





int half_StackDump(const Stack *st, const char *out_file_name)
{
    FILE *out_f = NULL;
    if (out_file_name == "_!stdout!_")
        out_f = stdout;
    else
        out_f = fopen(out_file_name, "a");

    assert(out_f != NULL);
    fprintf(out_f, "capacity = %d\n", this->capacity);
    fprintf(out_f, "size     = %d\n", this->size);

    for (int i = 0; i < this->capacity; i++)
        fprintf(out_f, "[%d] = %d\n", i, this->data[i]);

    fprintf(out_f, "canary#1 [-1] = %d\n", this->data[-1]);
    fprintf(out_f, "canary#2 [%d] = %d\n", this->capacity, this->data[this->capacity]);

    if (out_file_name != "_!stdout!_")
        fclose(out_f);


}

int StackDump(const Stack *st, const char *out_file_name, int pr)
{
    half_StackDump(st, out_file_name);

    if (pr == PRINT_CON)
        half_StackDump(st, "_!stdout!_");

    return 0;
}


int StackCap_x05(Stack *st)
{
    ST_ASSERT(st);

    BY_DESIGN(this->data = (st_t*)realloc(this->data - 1, (this->capacity / 2 + 2) * sizeof(st_t)) + 1);
    this->capacity /= 2;
    BY_DESIGN(this->data[this->capacity] = BIRD);

    ST_ASSERT(st);
    return 0;
}

int StackCap_x2(Stack *st)
{
    ST_ASSERT(st);

    BY_DESIGN(this->data = (st_t*)realloc(this->data - 1, (this->capacity * 2 + 2) * sizeof(st_t)) + 1);  //REALLOC ���� ����� ������� NULL - fixed in StackOk
    this->capacity *= 2;
    BY_DESIGN(this->data[this->capacity] = BIRD);

    ST_ASSERT(st);
    return 0;
}

int StackCap(const Stack *st)
{
    return this->capacity;
}

int Stacksize(const Stack *st)
{
    return this->size;
}


int UnitTest(Stack *st)
{

    StackCtor(st, 10);
    StackPush(st, 4);
    StackPush(st, 5);
    StackPush(st, 666);
    assert(StackPop(st) == 666);
    ST_ASSERT(st);
    assert(StackPop(st) == 5);
    assert(StackPop(st) == 4);
    StackDtor(st);

    StackCtor(st, 10);
    StackPush(st, 4);
    assert(StackPop(st) == 4);
    StackPush(st, 5);
    assert(StackPop(st) == 5);
    StackPush(st, 666);
    assert(StackPop(st) == 666);
    StackDtor(st);

    StackCtor(st, 270);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 202);
    assert(StackPop(st) == 202);
    assert(StackCap(st) == 135);
    assert(StackPop(st) == -3443);
    assert(StackPop(st) == 748);
    StackPush(st, 11);
    StackPush(st, 7);
    assert(StackPop(st) == 7);
    assert(StackPop(st) == 11);
    assert(StackPop(st) == 4);
    assert(Stacksize(st) == 0);
    StackDtor(st);

    StackCtor(st, 2);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    assert(StackPop(st) == 202);
    assert(StackPop(st) == 203);
    assert(Stacksize(st) == 3);
    assert(StackCap(st) == 8);
    StackDtor(st);

    StackCtor(st, 3);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    assert(Stacksize(st) == 10);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 202);
    assert(Stacksize(st) == 9);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 203);
    assert(Stacksize(st) == 8);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == -3443);
    assert(Stacksize(st) == 7);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 748);
    assert(Stacksize(st) == 6);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 4);
    assert(Stacksize(st) == 5);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 202);
    assert(Stacksize(st) == 4);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 203);
    assert(Stacksize(st) == 3);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == -3443);
    assert(Stacksize(st) == 2);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 748);
    assert(Stacksize(st) == 1);
    assert(StackCap(st) == 3);
    assert(StackPop(st) == 4);
    assert(Stacksize(st) == 0);
    assert(StackCap(st) == 3);
    StackDtor(st);

    StackCtor(st, 3);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    assert(Stacksize(st) == 5);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 202);
    assert(Stacksize(st) == 4);
    StackPush(st, 747);
    assert(Stacksize(st) == 5);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 747);
    assert(Stacksize(st) == 4);
    StackPush(st, 19);
    assert(Stacksize(st) == 5);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 19);
    assert(Stacksize(st) == 4);
    StackPush(st, 199);
    StackPush(st, 1999);
    StackPush(st, 19999);
    assert(Stacksize(st) == 7);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 19999);
    assert(Stacksize(st) == 6);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 1999);
    assert(Stacksize(st) == 5);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 199);
    assert(Stacksize(st) == 4);
    StackDtor(st);
    return 0;
}
