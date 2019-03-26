#define MEOW ;
#include "../my_std_def.h"
#include "my_excpt.h"


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
    atexit(close_new_del);

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