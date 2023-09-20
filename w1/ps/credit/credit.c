#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long num = get_long("Number: ");
    long check_sum = 0;
    //long test_sum = 0;
    for (long i = 100; i <= 10000000000000000; i = i * 100)
    {

        // Testing for first number set
        //long mod_ = (num % i);
        //long thing = mod_ / (i / 10);
        //test_sum += thing * 2;
        //printf("mod_: %li\n", mod_);
        //printf("thing: %li\n", thing);

        // starting with second to last, *2
        check_sum += (num % i) / (i / 10) * 2;

        // other digits
        //long ten_pow = i / 10;
        //long mod = num % ten_pow;
        //long other_ten = i / 100;
        //long sum = mod / other_ten;

        //printf("mod: %li\nsum: %li\n", mod, sum);

        check_sum += (num % (i / 10)) / (i / 100);
    }
    // printf("check_sum: %li\n",check_sum);

    // Check for card length and starting digits


    if (check_sum % 10 == 0)
    {
        // printf("valid\n");

        // Card length and starting digits
        if (num >= 340000000000000 && num <= 349999999999999)
        {
            printf("AMEX\n");
        }
        else if (num >= 370000000000000 && num <= 379999999999999)
        {
            printf("AMEX\n");
        }
        else if (num >= 5100000000000000 && num <= 5599999999999999)
        {
            printf("MASTERCARD\n");
        }
        else if (num >= 4000000000000000 && num <= 4999999999999999)
        {
            printf("VISA\n");
        }
        else if (num >= 4000000000000 && num <= 4999999999999)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    //printf("check_sum: %li\n",check_sum);

    //printf("test_sum: %li\n",test_sum);

}
