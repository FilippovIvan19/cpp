#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int vec_t;
#include "../vector/vector.h"

class my_excpt
{
    public:
        int err_;
        my_excpt *reason_;
        char *text_;
        int line_;
        my_excpt(int err, const char *text, my_excpt *reason, int line);
       ~my_excpt();
        void print(const FILE *file);
};




int func_2(Vector &v)
{
    try
    {
        if (v.capacity() <= 4)
            throw new my_excpt(4, "capacity <= 4", nullptr, __LINE__);
        if (v.capacity() == 7)
            throw new my_excpt(7, "capacity == 7", nullptr, __LINE__);
        v[4] = 16;
    }
    catch(my_excpt *err)
    {
        throw new my_excpt(2, "exception in func_2", err, __LINE__);
    }

    return rand() % 2;//if all is ok
}

int func_1(Vector &v)
{
    v[0] = 6;

    try
    {
        if (func_2(v))
            throw new my_excpt(3, "func_2 has returned 1", nullptr, __LINE__);
    }
    catch(my_excpt *err)
    {

        throw new my_excpt(1, "exception in func_1", err, __LINE__);
    }

    return 0;
}



int main()
{
    FILE *out_file = stdout;

    try
    {
        srand(time(nullptr));
        Vector v(rand() % 7 + 1);
        printf("%d\n", v.capacity());
        func_1(v);
    }




    catch(my_excpt *err)
    {
        err->print(out_file);
        delete err;
    }
    catch(...)
    {
        fprintf(out_file, "unchecked exception type was catched\n");
    }

    return 0;
}





//---------------------------------------------------------------------
//! my_excpt constructor
//---------------------------------------------------------------------
my_excpt::my_excpt(int err, const char *text, my_excpt *reason, int line):
    err_(err),
    reason_(reason),
    text_(new char [strlen(text)]),
    line_(line)
    {
        strcpy(this->text_, text);
    }


//---------------------------------------------------------------------
//! my_excpt destructor
//---------------------------------------------------------------------
my_excpt::~my_excpt()
{
    delete [] this->text_; //for canary
    this->text_ = nullptr;
    if (this->reason_ == nullptr)
        delete this->reason_;
    this->reason_ = nullptr;
}

//---------------------------------------------------------------------
//! prints info about exception into file
//!
//! @param in [file]          file descriptor for output file
//---------------------------------------------------------------------
void my_excpt::print(const FILE *file)
{
    fprintf((FILE*)file, "exception in line %d\n", this->line_);
    fprintf((FILE*)file, "%s\n", this->text_);
    if (this->reason_ == nullptr)
        return;
    fprintf((FILE*)file, "reason:\n");
    this->reason_->print((FILE*)file);
}
