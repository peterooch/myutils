#include <stdio.h>

#define GET_THREE_BITS(x) (x & 0b111)
#define GET_FOUR_BITS(x) (x & 0b1111)
#define GET_1_BIT(x) (x & 0b1)
#define GET_2_BIT(x) (x & 0b10)
#define GET_3_BIT(x) (x & 0b100)
#define GET_4_BIT(x) (x & 0b1000)
#define GET_CARRY(x) (x & 0b10000)

/* Copyright: Baruch Rutman (peterooch at gmail dot com)
 * Written in May 22nd 2019
 */

int FourBitAdder(int a, int b, int* carry)
{
    int result;

    result = GET_FOUR_BITS(a) + GET_FOUR_BITS(b) + *carry;

    *carry = GET_CARRY(result);

    //printf("dbg: a = %d, b = %d, carry = %d, res = %d\n", a, b, *carry, result);

    return GET_FOUR_BITS(result);
}

/*
 * implements a 3bit * 3bit multiplier while using a pseudo 4 bit adder
 */
int calc(int a, int b)
{
    int res, final_res, carry = 0, carry2 = 0;

    //scanf("%d%d", &a, &b);

    res = FourBitAdder(GET_1_BIT(b) ? GET_THREE_BITS(a) : 0, GET_2_BIT(b) ? (GET_THREE_BITS(a) << 1) : 0, &carry);

    final_res = GET_1_BIT(res);

    res = FourBitAdder(GET_THREE_BITS((res >> 1)) + GET_4_BIT((carry >> 1)), GET_3_BIT(b) ? (GET_THREE_BITS(a) << 1) : 0, &carry2);

    final_res += (carry2 << 1) + (res << 1);

    return final_res;
}

int main()
{
    for (int i = 0; i <= 0b111; i++)
    {
        for (int j = 0; j <= 0b111; j++)
        {
            printf("%d * %d = %d\n", i, j, calc(i,j));
        }
    }
}