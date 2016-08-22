#include "utils.h"
#include <math.h>

using namespace std;

bool isPrime(unsigned long number)
{
	if(number == 0 || number == 1)
	{
		return false;
	}
	else if(number == 2)
	{
		return true;
	}
	else if(number % 2 == 0)
	{
		return false;
	}
	else
	{
		bool isprime = true;
		for(unsigned long i = 3 ; i <= sqrt(number) && isprime ; i += 2)
		{
			if(number % i == 0)
			{
				isprime = false;
			}
		}
		return isprime;
	}
}


unsigned long prev_prime(unsigned long number)
{
	//returns a prime less than a number and kept 2 for 1 and 0
	if(number == 0 || number == 1 || number == 2 || number == 3)
	{
		return 2;
	}
	else if(number % 2 == 0)
	{
		number--;
		while(true)
		{
			if(isPrime(number))
			{
				break;
			}
			number -= 2;
		}
		return number;
	}
	else
	{
		number -= 2;
		while(true)
		{
			if(isPrime(number))
			{
				break;
			}
			number -= 2;
		}
		return number;
	}
}


unsigned long myhash(string str,unsigned long prime)
{
	unsigned char * ch = (unsigned char *)str.c_str();
	unsigned long hash = 0;
	int c;

	while ((c = *ch++))
	{
		hash = (c + (hash << 16) + (hash << 6) - hash) % prime; //source sleepycat
	}

	return hash % prime;
}