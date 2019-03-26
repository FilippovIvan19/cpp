#include <stdio.h>

int convert(int dec_val, int osn)
{
    int rez = 0;
    int coeff = 1;
    while (dec_val != 0)
    {
        rez += coeff * (dec_val % osn);
        dec_val /= osn;
        coeff *= 10;
    }

    return rez;
}


int main()
{
    int osn;
    int dec_val;
    printf("введите основание\n");
    scanf("%d", &osn);
    printf("введите число\n");
    scanf("%d", &dec_val);

    int osn_val = convert(dec_val, osn);

    printf("%d\n", osn_val);


    return 0;
}
