#include <stdio.h>
#include <stdlib.h>


#ifdef MEOW
    const FILE* new_delete_file = fopen("new_delete", "w");
    void* allocated_memory[100];
    bool  allocated_memory_bool[100];
    int   allocated_memory_size = 0;
    #include "renew/new_del_control.h"


    #include <iostream>
    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";


    #define assert( check )\
        if(!(check))\
        {\
            printf("OOPS: %s in %s, function %s\n, line %d", #check, __FILE__, __PRETTY_FUNCTION__, __LINE__);\
            abort();\
        }


    #define new new (__FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
    #define $(x) ;
    #define assert( check ) ;
#endif



//---------------------------------------------------------------------
//! final processing new/delete
//---------------------------------------------------------------------
void close_new_del()
{
    #ifdef MEOW
        fprintf((FILE*)new_delete_file, "\nundeleted memory:\n");

        for (int i = 0; i < allocated_memory_size; i++)
            if (allocated_memory_bool[i])
                fprintf((FILE*)new_delete_file, "%p, ", allocated_memory[i]);
        fprintf((FILE*)new_delete_file, "\n");
        fclose((FILE*)new_delete_file);
    #endif
}