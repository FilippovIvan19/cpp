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
            StackDump(x, "assertstat.txt", 0);\
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

#define st_t int
#define BY_DESIGN(x) x//it's a canary


const int LcAll = 0;
const int BIRD = -1245666;
const int POISON = -66613666;
const int PRINT_CON = 1;
const int NPRINT_CON = 0;


struct Stack
{
    st_t *data;
    int size;
    int capacity;
};


//---------------------------------------------------------------------
//! check if stack is available to use
//!
//! @param in [st]          pointer to stack
//!
//! @returns bool value is stack ok or not
//---------------------------------------------------------------------
bool StackOk(const Stack *st);

//---------------------------------------------------------------------
//! create stack
//!
//! @param in [st]           pointer to stack
//! @param in [len]          length of creating stack
//!
//! @returns pointer to stack
//---------------------------------------------------------------------
Stack *StackCtor(Stack *st, int len);

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


int Change_StackCap(Stack *st, int coeff)
{
    ST_ASSERT(st);
    assert(coeff != 0 && coeff != 1 && coeff != -1);

    if (coeff > 0)
    {
        BY_DESIGN(st->data = (st_t*)realloc(st->data - 1, (st->capacity * coeff + 2) * sizeof(st_t)) + 1);
        st->capacity *= coeff;
    }
    else
    {
        BY_DESIGN(st->data = (st_t*)realloc(st->data - 1, (st->capacity / -coeff + 2) * sizeof(st_t)) + 1);
        st->capacity /= -coeff;
    }

    BY_DESIGN(st->data[st->capacity] = BIRD);
    ST_ASSERT(st);
    return 0;
}

bool StackOk(const Stack *st)
{
    return (st != NULL)
        && (st->data != NULL) && (st->data != (int*)-1)
        && (st->capacity > 0)
        && (0 <= st->size) && (st->size <= st->capacity)
        BY_DESIGN(&& (st->data[st->capacity] == BIRD) && (st->data[-1] == BIRD));     ///�������� (ST->CAPACITY > 0) ������
    /// ��� ��� ���� (ST->size >= 0) && (ST->size <= ST->CAPACITY) - �� ������ �.�. ����� �� ����������� ������ ��������� ����
 ///��� �������� �� ��������� ������ � ����� - �� �����������
 ///��� �������� - fixed
}

Stack *StackCtor(Stack *st, int len)
{
    st->data = (st_t*)calloc(len + 2, sizeof(st_t)) + 1;  //!! � ���� ������ NULL - fixed in StackOk

    st->capacity = len;
    st->size = 0;

    BY_DESIGN(st->data[-1]           = BIRD);
    BY_DESIGN(st->data[st->capacity] = BIRD);

    ST_ASSERT(st);

    return st;
}

int half_StackDump(const Stack *st, const char *out_file_name)
{
    FILE *out_f = NULL;
    if (out_file_name == "_!stdout!_")
        out_f = stdout;
    else
        out_f = fopen(out_file_name, "a");

    assert(out_f != NULL);
    fprintf(out_f, "capacity = %d\n", st->capacity);
    fprintf(out_f, "size     = %d\n", st->size);

    for (int i = 0; i < st->capacity; i++)
        fprintf(out_f, "[%d] = %d\n", i, st->data[i]);

    fprintf(out_f, "canary#1 [-1] = %d\n", st->data[-1]);
    fprintf(out_f, "canary#2 [%d] = %d\n", st->capacity, st->data[st->capacity]);

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

int StackDtor(Stack *st)
{
    ST_ASSERT(st);

    for (int i = -1; i <= st->capacity; i++)
        st->data[i] = POISON;

    st->size = -1;
    st->capacity = -1;

    free(st->data - 1);
    st->data = (int*)-1;

    return 0;
}

int StackPush(Stack *st, st_t x)
{
    ST_ASSERT(st);

    if (st->size == st->capacity)
        Change_StackCap(st, 2);

    st->size++;
    st->data[st->size - 1] = x;

    ST_ASSERT(st);

    return 0;
}

int StackCap_x05(Stack *st)
{
    ST_ASSERT(st);

    BY_DESIGN(st->data = (st_t*)realloc(st->data - 1, (st->capacity / 2 + 2) * sizeof(st_t)) + 1);
    st->capacity /= 2;
    BY_DESIGN(st->data[st->capacity] = BIRD);

    ST_ASSERT(st);
    return 0;
}

int StackCap_x2(Stack *st)
{
    ST_ASSERT(st);

    BY_DESIGN(st->data = (st_t*)realloc(st->data - 1, (st->capacity * 2 + 2) * sizeof(st_t)) + 1);  //REALLOC ���� ����� ������� NULL - fixed in StackOk
    st->capacity *= 2;
    BY_DESIGN(st->data[st->capacity] = BIRD);

    ST_ASSERT(st);
    return 0;
}

int StackCap(const Stack *st)
{
    return st->capacity;
}

int Stacksize(const Stack *st)
{
    return st->size;
}

st_t StackPop(Stack *st)
{
    ST_ASSERT(st);
    st->size--;
    ST_ASSERT(st);

    st_t x = st->data[st->size];
    if ((st->size * 4 < st->capacity) && (st->capacity > 3))
        Change_StackCap(st, -2);

    ST_ASSERT(st);
    return x;
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
///���� ����� �� �������� ��������� ��������: ������ �� ������� ����� � � � - ��� � �� ������ ������ �����
/// ��� ������ ���������, ��� �� �������� (�������)

    return 0;
}
