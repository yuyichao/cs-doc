main()
{
    int num=0x41414141;

    printf("Before: num = %p \n", num);
    printf("%.20d%n\n", num, &num);
    printf("After:  num = %p \n", num);
}
