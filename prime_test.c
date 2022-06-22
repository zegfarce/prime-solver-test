/* get gcc to include POSIX extensions in c90 mode */
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Number base to use for testing. */
#define BASE 6

/* The first number that fails the easier test. */
#define FIRST_EASY_FAIL 121

/* Digit tests (unique to each number base) */
/* Specifically for cases where the digit tells you if it's prime. */
/* e.g. in base 6, if a number doesn't end in 1 or 5, it's composite. */
#define DIGIT_TESTS(x) (x != 1 && x != 5)

int testPrimeEasy(unsigned int num) {
	unsigned int numread = num; /* Number to generate next modulo from */
	unsigned int mod = num % BASE; /* Current modulo */

	/* check if it doesn't end in obvious prime factors */
	if (DIGIT_TESTS(mod))
		return 0;

	/* adding digits test (length = 1) */
	/* this is to check for factors of BASE-1. */
	while (numread >= BASE) {
		mod = numread % BASE;
		while (numread /= BASE)
			mod += numread % BASE;
		numread = mod;
	}

	/* check if prime factors are same as in 5 */
	/* 5 has the prime factors (5). */
	/* we don't have to do a modulous here because */
	/* modres necessarily falls within the range of [0, 5] */
	if (mod == BASE - 1)
		return 0;

	/* adding digits test (length = 2) */
	/* this is to check for factors of BASE+1. */
	numread = num;
	while (numread >= BASE*BASE) {
		mod = numread % (BASE*BASE);
		while (numread /= (BASE*BASE))
			mod += numread % (BASE*BASE);
		numread = mod;
	}

	/* similar check as the factors for 5 above, */
	/* except that mod necessarily falls within the range [0, 35] */
	/* which contains multiple numbers with 7 as a factor */
	if (mod % (BASE + 1) == 0)
		return 0;

	/* well. guess it's probably prime then */
	return 1;
}

int testPrimeHard(unsigned int num) {
	unsigned int i;

	for (i = 3; i < num; i += 2)
		if (!(num % i))
			return 0;

	return 1;
}

static __inline int firstPrimeTest(unsigned int num) {
	return !(num <= 1 || (!(num & 1) && num != 2));
}

int testPrime(unsigned int num) {
	if (!firstPrimeTest(num))
		return 0;
	
	/* base-6 easy tests for small numbers */
	if (num > BASE && num < FIRST_EASY_FAIL)
		return testPrimeEasy(num);
	/* classic prime solver */
	else
		return testPrimeHard(num);
}

/* generates a string, rendering num in the specified base (positional). */
char *toBaseN(unsigned int num, unsigned int base) {
	/* initially allow 1 char */
	unsigned int len = 2;
	char *tmpstr, *str;
	unsigned int mod;
	unsigned int i;

	/* bases above 10 are outside the scope */
	/* sorry, dozenal */
	if (base > 10)
		return NULL;

	/* Alloc after check to prevent memory leaks */
	tmpstr = calloc(len, sizeof(char));

	if (!tmpstr)
		return NULL;
	
	tmpstr[0] = '0'; /* default 0 */

	if (!num)
		return tmpstr;

	/* string construction */
	
	/* do modified first iteration outside of loop */
	/* because realloc isn't required */
	mod = num % base;
	tmpstr[0] = '0' + mod;

	/* go through the rest of the digits */
	while (num /= base) {
		str = (char *)realloc(tmpstr, ++len);

		if (!str)
			return NULL;

		mod = num % base;

		/* terminate string, then apply value */
		tmpstr[len - 1] = '\0';
		tmpstr[len - 2] = '0' + mod;
	};

	str = (char *)calloc(len, sizeof(char));

	/* reverse the string */
	for (i = 0; i < len - 1; i++) {
		/* len - 2 to skip over null char, and also i starts at 0 */
		str[i] = tmpstr[len - 2 - i];
	}

	free(tmpstr);

	return str;
}

/* calculate the average time for running testPrime */
void runAverage(int start, int end) {
	time_t avg_sec = 0;
	long avg_nsec = 0;
	unsigned int iter = 0;
	unsigned int i;

	for (i = start; i < end; i++) {
		char *str = toBaseN(i, BASE);
		unsigned int isPrime;
		clock_t t;
		struct timespec start, end;
		time_t sec;
		long nsec;

		if (!str)
			return;

		/* test the prime, time performance */
		t = clock_gettime(CLOCK_MONOTONIC, &start);
		isPrime = testPrime(i);
		t = clock_gettime(CLOCK_MONOTONIC, &end);

		/* calculate the difference... */
		sec = end.tv_sec - start.tv_sec;
		nsec = end.tv_nsec - start.tv_nsec;

		/* ...accounting for carry and borrow. */
		if (nsec < 0) {
			++sec;
			nsec += 1000000000;
		} else if (nsec >= 1000000000) {
			--sec;
			nsec -= 1000000000;
		}
		
		/* printf("%c %d (%s), %d.%09ld\n", isPrime ? '*' : ' ', i, str, sec, nsec); */
		printf("%c", isPrime ? '*' : '-');
		/* write immediately to make it look cool! */
		fflush(stdout);

		/* then calc the average */
		++iter;
		avg_nsec += nsec/iter;
		/* sometimes there's integer seconds we get to average */
		nsec = sec*1000000000;
		avg_nsec += nsec/iter;
		/* gotta do this just in case we take ages */
		avg_sec += sec/iter;

		/* Send a newline every 2^5 == 32 chars */
		if (iter && !(iter & ((1<<5)-1)))
			printf("\n", iter);
		
		free(str);
	}

	printf("\n");
	printf("avg %d.%09ld seconds per prime test\n", avg_sec, avg_nsec);
}

/* Helper function to find the first number where testPrimeEasy fails. */
int findFirstFail() {
	unsigned int i = BASE + 1;

	while (!firstPrimeTest(i) || testPrimeEasy(i) == testPrimeHard(i))
		i++;

	return i;
}

int main(int argc, char *argv[]) {
	printf("Easy prime check first fails at %d\n", findFirstFail());

	runAverage(0, FIRST_EASY_FAIL);
	runAverage(FIRST_EASY_FAIL, FIRST_EASY_FAIL * 2);

	return 0;
}
