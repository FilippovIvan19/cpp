//---------------------------------------------------------------------
//! prints string to stdout
//!
//! @param in [str]          string to print
//---------------------------------------------------------------------
void prind(const char* str)
{
    printf("%s", str);
}

//---------------------------------------------------------------------
//! works like printf but understand types and use #instead of %_
//!
//! @param in [str]          string to print
//! @param in [arg]          first argument after string
//! @param in [args]         other arguments 
//---------------------------------------------------------------------
template <typename Type, typename... Others>
void prind(const char* str, Type arg, Others... args)
{
    while (*str && *str != '#')
    {
        putchar(*str);
        ++str;
    }

    if (!*str)
        return;

    ++str;
    std::cout << arg;
    prind(str, args...);
}