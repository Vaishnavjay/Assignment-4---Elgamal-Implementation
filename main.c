#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int g, y, p, x, cipher1, cipher2;

int eea(int b, int n, int* y, int* a)
{
    if (b == 1) {
        *y = (*y * (*a)) % n;
    }

    *a = (*a) * (*a) % n;
}
int elgamal(int a, int b, int c)
{
    int r;
    int y = 1;

    while (b > 0)
    {
        r = b % 2;
        eea(r, c, &y, &a);
        b = b / 2;
    }
    return y;
}
int findGCD(int n1, int n2) {
	int i;
	int gcd;

	for(i = 1; i <= n1 && i <= n2; ++i) {
		if(n1 % i == 0 && n2 % i == 0)
			gcd = i;
	}

	return gcd;
}
int keygen()
{
    p = 1229;
    g = 2;
    x = 1097;
    y = elgamal(g, x, p);
}

FILE *out1, *out2;
int encrypt(int m)
{
    out1 = fopen("cipher1.txt", "a+");
    out2 = fopen("cipher2.txt", "a+");

    int r;
    do {
        r = rand() % (p - 1) + 1;
    }
    while (findGCD(r, p) != 1);

    cipher1 = elgamal(g, r, p);
    cipher2 = m * elgamal(y, r, p)% p;

    fprintf(out1, "%d ", cipher1);
    fprintf(out2, "%d ", cipher2);

    fclose(out1);
    fclose(out2);
}
int decrypt(int C1, int C2)
{
    int dec = C2 * elgamal(C1, p - 1 - x, p) % p;
    printf("%c",dec);
}
int main()
{
    FILE *message, *ciph1, *ciph2;
    keygen();
    message = fopen("message.txt", "r+");
    if (message == NULL)
    {
        printf("Error opening Source File.\n");
        exit(1);
    }
    printf("\nMessage to be encrypted is : \n");
    char ch;
    while (1)
    {
        ch = getc(message);
        printf("%c",ch);
        if (ch == EOF) {
            break;
        }

        encrypt(toascii(ch));
    }
    printf("\n\nCipher Text C1 is : ");
    ciph1 = fopen("cipher1.txt", "r");
    while (1)
    {
        ch = getc(ciph1);
        printf("%c",ch);
        if (ch == EOF) {
            break;
        }
    }
    printf("\n\nCipher Text C2 is : ");
    ciph2 = fopen("cipher2.txt", "r");
    while (1)
    {
        ch = getc(ciph2);
        printf("%c",ch);
        if (ch == EOF) {
            break;
        }
    }
    fclose(ciph1);
    fclose(ciph2);
    fclose(message);
    int C1, C2;
    ciph1 = fopen("cipher1.txt", "r");
    ciph2 = fopen("cipher2.txt", "r");
    printf("\n\nDecrypted Message is : \n");
    while (1)
    {
        int ret = fscanf(ciph1, "%d", &C1);
        fscanf(ciph2, "%d", &C2);
        if (ret == -1) {
            break;
        }
        decrypt (C1, C2);
    }

    fclose(ciph1);
    fclose(ciph2);
    remove("cipher1.txt");
    remove("cipher2.txt");
    printf("\n\n");
    getch();
    return 0;
}
