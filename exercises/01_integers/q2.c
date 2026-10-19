#include <stdio.h>
void f(int a[])
{
    printf("%zu\n", sizeof(a));
}

int main(void)
{
    int arr[4] = {10, 20, 30, 40};
    printf("%zu\n", sizeof(arr));   /* prints 16  (4 ints x 4 bytes) */
    f(arr);
    return 0;
}
