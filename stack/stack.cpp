#define MEOW ;
#include "../my_std_def.h"

typedef int st_t;
#include "stack.h"


int main()
{
    atexit(close_new_del);
    


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