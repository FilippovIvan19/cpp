void* operator new (size_t size, const char *file, const char *function, int line)
{
    void *p = new char [size];

    fprintf((FILE*)new_delete_file, "%p new      used in: file %s, function %s, line %d\n", p, file, function, line);

    int ind = -1;
    for (int i = 0; i < allocated_memory_size; i++)
        if (allocated_memory[i] == p)
            ind = i;

    if (ind == -1)
    {
        allocated_memory[allocated_memory_size] = p;
        allocated_memory_bool[allocated_memory_size] = 1;
        allocated_memory_size++;
    }
    else
        allocated_memory_bool[ind] = 1;

    return p;
}

void* operator new[] (size_t size, const char *file, const char *function, int line)
{

    void *p = new char [size];

    fprintf((FILE*)new_delete_file, "%p new[]    used in: file %s, function %s, line %d\n", p, file, function, line);

    int ind = -1;
    for (int i = 0; i < allocated_memory_size; i++)
        if (allocated_memory[i] == p)
            ind = i;

    if (ind == -1)
    {
        allocated_memory[allocated_memory_size] = p;
        allocated_memory_bool[allocated_memory_size] = 1;
        allocated_memory_size++;
    }
    else
        allocated_memory_bool[ind] = 1;

    return p;
}

void operator delete (void *p) noexcept
{

    int ind = -1;
    for (int i = 0; i < allocated_memory_size; i++)
        if (allocated_memory[i] == p)
            ind = i;

    if (ind == -1)
    {
        fprintf((FILE*)new_delete_file, "%p, ERROR DELETING NOT DYNAMIC MEMORY\n", p);
        printf("%p, ERROR DELETING NOT DYNAMIC MEMORY\n", p);
        //abort();
    }
    else
        if (allocated_memory_bool[ind] == 0)
        {
            fprintf((FILE*)new_delete_file, "%p, ERROR 2ND DELETING MEMORY\n", p);
            printf("%p, ERROR 2ND DELETING MEMORY\n", p);
            //abort();
        }
        else
            allocated_memory_bool[ind] = 0;


    fprintf((FILE*)new_delete_file, "%p delete   used\n", p);
    free(p);
}

void operator delete[] (void *p) noexcept
{

    int ind = -1;
    for (int i = 0; i < allocated_memory_size; i++)
        if (allocated_memory[i] == p)
            ind = i;

    if (ind == -1)
    {
        fprintf((FILE*)new_delete_file, "%p, ERROR DELETING NOT DYNAMIC MEMORY\n", p);
        printf("%p, ERROR DELETING NOT DYNAMIC MEMORY\n", p);
        //abort();
    }
    else
        if (allocated_memory_bool[ind] == 0)
        {
            fprintf((FILE*)new_delete_file, "%p, ERROR 2ND DELETING MEMORY\n", p);
            printf("%p, ERROR 2ND DELETING MEMORY\n", p);
            //abort();
        }
        else
            allocated_memory_bool[ind] = 0;


    fprintf((FILE*)new_delete_file, "%p delete[] used\n", p);
    free(p);
}
