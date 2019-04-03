#define MEOW ;
#include "../my_std_def.h"

typedef int vec_t;
#include "vector_+bool.h"





int main()
{
    atexit(close_new_del);
    


    /*Vector<int> v1(10);
    Vector<> v2;
    printf("%d\n", v2.capacity());
    v2 = v1;
    v2.resize(16);
    v2.push_back(12);
    printf("%d\n", v2.back());
    printf("%d\n", v2.front());
    int a = v2.pop_back();
    Vector<int> v3 (v2);
    printf("0\n\n");*/

    Vector<bool> vb(89);
    vb.push_back(1);
    vb.push_back(1);
    vb.push_back(1);
    //vb.resize(15);
    vb.push_back(0);
    vb.push_back(1);
    vb.push_back(1);
    vb.push_back(1);
    bool b = vb[1];
    //printf("%d\n", b);
    vb[80] = b;
    for (int i = 0; i < vb.capacity(); ++i)
    {
    	if (i % 8 == 0)
    		printf(" ");
    	printf("%d", (bool)vb[i]);
    }

    printf("\n");



    return 0;
}