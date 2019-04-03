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
    delete [] this->text_;
    this->text_ = nullptr;
    if (this->reason_ != nullptr)
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
