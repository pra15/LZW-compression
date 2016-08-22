#include <iostream>
#include <stdlib.h>
#include "compressor.h"

using namespace std;

int main()
{
	cout << "Welcome User, to the" << endl;
	cout << "	LZW compression and decompression manager" << endl << endl;
	
	cout << "press 0 --> to exit the manager" << endl;
	cout << "press 1 --> to compress a file" << endl;
	cout << "press 2 --> to recover back the original file" << endl;
	cout << "press 3 --> to display this message box" << endl << endl;
	
	
	while(true)
	{
		string i;
		cin >> i;
		if(i == "0")
		{
			cout << "Exiting manager ..." << endl;
			exit(0);
		}
		else if(i == "1")
		{
			string random;
			cout << "Please enter location of the file to be compressed" << endl;
			cin >> random;
			cout << "Please wait for a while ..." << endl;
			compress(random);
		}
		else if(i == "2")
		{
			string random;
			cout << "Please enter location of the file to be decompressed" << endl;
			cin >> random;
			cout << "Please wait for a while ..." << endl;
			uncompress(random);
		}
		else if(i == "3")
		{
			cout << endl << "press 0 --> to exit the manager" << endl;
			cout << "press 1 --> to compress a file" << endl;
			cout << "press 2 --> to recover back the original file" << endl;
			cout << "press 3 --> to display this message box" << endl << endl;
		}
		else
		{
			cout << "invalid_argument" << endl << endl;
		}
	}
}
