void prind(const char* str)
{
    printf("%s", str);
}
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