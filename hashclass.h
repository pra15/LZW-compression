#ifndef HASHCLASS_H
#define HASHCLASS_H

#include <iostream>
using namespace std;

class hashclass
{
	//class which is hash implementation of dictionary of LZW
	public:
		hashclass(unsigned int ini_size);	//constructor; argument for its initial size
		void insert(string str);							//inserts string
		void insert(string str,unsigned long hash_given);	//inserts string given its hash
		bool isExist(string str,unsigned long & position);	//checks for string and updates its key
		bool isExist(string str,unsigned long hash,unsigned long & position);
		bool isExist(unsigned long number,string & str);	//checks index and updates string at that
		unsigned int size_char();
		unsigned long elements_count;		//few public and private elements
		unsigned long prime;
	//private:
		bool * array;
		string * dictionary;
	private:
		unsigned long virtual_len;
};

#endif
