#include <stdio.h>
#include <math.h>
#include "hashclass.h"
#include "utils.h"

using namespace std;

hashclass::hashclass(unsigned int ini_size)
{
	virtual_len = pow(10,ini_size);
	prime = prev_prime(virtual_len);
	array = new bool[prime];
	for(unsigned long i = 0 ; i<prime ; i++)
	{
		array[i] = false;
	}
	dictionary = new string[prime];
	elements_count = 0;
	for(int i = 0 ; i < 256 ; i++)
	{
		string to_in;
		to_in = to_in + (char)i;
		insert(to_in);
	}	
}

void hashclass::insert(string str)
{
	//normal insertion till half full; heuristics:source internet
	//to avoid infinite loop scenario in quadratic probing
	if(elements_count < prime/2)
	{
		unsigned long hash = myhash(str,prime);
		if(array[hash])
		{
			unsigned long i = 1;
			unsigned long index = hash;
			while(array[index])
			{
				index = (index + i*i) % prime;
				i++;
			}
			array[index] = true;
			dictionary[index] = str;
		}
		else
		{
			array[hash] = true;
			dictionary[hash] = str;
		}
		elements_count++;
	}
	//resizing otherwise by copying previous elements
	else
	{
		unsigned long dummy = prev_prime(virtual_len * 2);
		bool * darray = new bool[dummy];
		for(unsigned long i = 0 ; i<dummy ; i++)
		{
			darray[i] = false;
		}
		string * dstring = new string[dummy];
		for(unsigned int k = 0 ; k < prime ; k++)
		{
			if(array[k])
			{
				unsigned long hash = myhash(dictionary[k],dummy);
				if(darray[hash])
				{
					unsigned long i = 1;
					unsigned long index = hash;
					while(darray[index])
					{
						index = (index + i*i) % dummy;
						i++;
					}
					darray[index] = true;
					dstring[index] = dictionary[k];
				}
				else
				{
					darray[hash] = true;
					dstring[hash] = dictionary[k];
				}
			}
		}
		delete [] array;
		delete [] dictionary;
		array = darray;
		dictionary = dstring;
		virtual_len = virtual_len * 2;
		prime = dummy;
		
		unsigned long hash = myhash(str,prime);
		if(array[hash])
		{
			unsigned long i = 1;
			unsigned long index = hash;
			while(array[index])
			{
				index = (index + i*i) % prime;
				i++;
			}
			array[index] = true;
			dictionary[index] = str;
		}
		else
		{
			array[hash] = true;
			dictionary[hash] = str;
		}
		elements_count++;
	}
}


void hashclass::insert(string str,unsigned long hash_given)
{
	//same as above just no need to calculate hash for very large strings
	if(elements_count < prime/2)
	{
		unsigned long hash = hash_given;
		if(array[hash])
		{
			unsigned long i = 1;
			unsigned long index = hash;
			while(array[index])
			{
				index = (index + i*i) % prime;
				i++;
			}
			array[index] = true;
			dictionary[index] = str;
		}
		else
		{
			array[hash] = true;
			dictionary[hash] = str;
		}
		elements_count++;
	}
	else
	{
		unsigned long dummy = prev_prime(virtual_len * 2);
		bool * darray = new bool[dummy];
		string * dstring = new string[dummy];
		for(unsigned int k = 0 ; k < prime ; k++)
		{
			if(array[k])
			{
				unsigned long hash = myhash(dictionary[k],dummy);
				if(darray[hash])
				{
					unsigned long i = 1;
					unsigned long index = hash;
					while(darray[index])
					{
						index = (index + i*i) % dummy;
						i++;
					}
					darray[index] = true;
					dstring[index] = dictionary[k];
				}
				else
				{
					darray[hash] = true;
					dstring[hash] = dictionary[k];
				}
			}
		}
		delete [] array;
		delete [] dictionary;
		array = darray;
		dictionary = dstring;
		virtual_len = virtual_len * 2;
		prime = dummy;
		
		unsigned long hash = hash_given;
		if(array[hash])
		{
			unsigned long i = 1;
			unsigned long index = hash;
			while(array[index])
			{
				index = (index + i*i) % prime;
				i++;
			}
			array[index] = true;
			dictionary[index] = str;
		}
		else
		{
			array[hash] = true;
			dictionary[hash] = str;
		}
		elements_count++;
	}
}



bool hashclass::isExist(string str,unsigned long & position)
{
	//finds str at hash(str), +1^2 , +2^2, ... till finds a null
	//infinite loop condition take care above
	bool toReturn = false;
	unsigned long i = 1;
	unsigned long index = myhash(str,prime);
	while(array[index] && !(toReturn))
	{
		if(dictionary[index] == str)
		{
			position = index;
			toReturn = true;
		}
		index = (index + i*i) % prime;
		i++;
	}
	return toReturn;
}

bool hashclass::isExist(string str,unsigned long hash,unsigned long & position)
{
	//same as above but no need to calculate hash
	bool toReturn = false;
	unsigned long i = 1;
	unsigned long index = hash;
	while(array[index] && !(toReturn))
	{
		if(dictionary[index] == str)
		{
			position = index;
			toReturn = true;
		}
		index = (index + i*i) % prime;
		i++;
	}
	return toReturn;
}


bool hashclass::isExist(unsigned long number,string & str)
{
	//just looks at the given location if exists updates str or makes it NULL
	str = dictionary[number];
	return array[number];
}


unsigned int hashclass::size_char()
{
	//char_size of numbers in dictionary
	return (unsigned int)snprintf(NULL, 0, "%lu", prime);
}