#include <stdio.h>
#include <math.h>

int isPrime(int n) {
	for (int i=2; i<=sqrt(n); i++) {
		if (n % i == 0)
			return -1;
	}

	return 1;
}

int main() {
    int n;

    while(1) {
        printf("Please input a number(negative means kill program):\n");
        scanf("%d", &n);
        if(n<0) {
            return 0;
        }
        if(isPrime(n) > 0) {
            printf("It's a prime number\n");
        }
        else {
            printf("It isn't a prime number\n");
        }
    }

	return 0;
}
