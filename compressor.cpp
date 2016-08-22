#include <iostream>
#include <fstream>
#include <bitset>
#include <math.h>
#include <stdlib.h>
#include "hashclass.h"
#include "compressor.h"

using namespace std;


void print(FILE * pointer,unsigned long to_print,unsigned int size)
{
	if(size - (unsigned int)snprintf(NULL, 0, "%lu", to_print) > 1)
	{
		fprintf(pointer,"%lu",to_print);
		fputc(59,pointer);
	}
	else
	{
		while((unsigned int)snprintf(NULL, 0, "%lu", to_print) != size)
		{
			fputc(48,pointer);
			size--;
		}
		fprintf(pointer,"%lu",to_print);
	}
}


void double_compress(string name)
{
	//changing keys to acheive compression using LZW
	//could have set in compress() also but dividing it 
	//makes it simple and elegant
	FILE * pointer;
	pointer = fopen((name+".dcmp").c_str(),"r");
	if (pointer == NULL) 
	{
		cout << "Unable to open file" << endl; 
		exit(-1);
	}
	FILE * output;
	output = fopen((name+".cmp").c_str(),"w");
	if (output == NULL) 
	{
		cout << "Unable to create file" << endl; 
		exit(-1);
	}
	rewind(pointer);
	
	while(true)
	{
		int ch = getc(pointer);
		if(ch == EOF)
		{
			break;
		}
		else
		{
			ch = ch - 48;
			ch = ch << 4;
			int ch2 = getc(pointer);
			if(ch2 == EOF)
			{
				ch = ch + ((15));
				fputc(ch,output);
				break;
			}
			else
			{
				ch = ch + ((ch2 - 48));
				fputc(ch,output);
			}
			
		}
	}
	fcloseall();
}



void initial_uncompress(string name)
{	
	//reverting keys
	//could have done in uncompress() also 
	//but dividing it makes it simple and elegant
	FILE * pointer;
	pointer = fopen(name.c_str(),"r");
	if (pointer == NULL) 
	{
		cout << "Unable to open file" << endl; 
		exit(-1);
	}
	FILE * output;
	output = fopen((name+"d").c_str(),"w");
	if (output == NULL) 
	{
		cout << "Unable to create file" << endl; 
		exit(-1);
	}
	rewind(pointer);
	
	while(true)
	{
		int ch = getc(pointer);
		if(ch == EOF)
		{
			break;
		}
		else
		{
			bitset<4> bit_1(ch);
			int ch1 = (int)bit_1.to_ulong() + 48;
			ch = (ch >> 4);
			ch = ch + 48;
			fputc(ch,output);
			if(ch1 == 63)
			{
				break;
			}
			else
			{
				fputc(ch1,output);
			}
		}
	}
	fcloseall();
}





void compress(string name)
{
	//just to get effective results some raw analysis used
	//just a refinement
	unsigned int start;
	ifstream is;
	is.open (name.c_str(), ios::binary );
	is.seekg (0, ios::end);
	int length = is.tellg();
	is.close();
	if(length < 2*11000)
	{
		start = 4;
	}
	else if(length < 3*153600)
	{
		start = 5;
	}
	else if(length < 4*2516000)
	{
		start = 6;
	}
	else
	{
		start = 7;
	}
	
	FILE * pointer;
	pointer = fopen(name.c_str(),"r");
	if (pointer == NULL) 
	{
		cout << "Unable to open file" << endl; 
		exit(-1);
	}
	FILE * output;
	output = fopen((name+".dcmp").c_str(),"w");
	if (output == NULL) 
	{
		cout << "Unable to create file" << endl; 
		exit(-1);
	}
	rewind(pointer);
	
	hashclass * mydictionary;
	mydictionary = new hashclass(start);
	unsigned int prev_size = mydictionary->size_char();
	fputc(58,output);
	fprintf(output,"%u",prev_size);
	fputc(58,output);
	bool no_EOF = true;
	
	while(no_EOF)
	{
		if(mydictionary->elements_count + 10 > (mydictionary->prime)/2)
		{
			delete [] mydictionary->array;
			delete [] mydictionary->dictionary;
			delete mydictionary;
			mydictionary = new hashclass(start);
			fputc(60,output);
		}
		string str;
		unsigned long hash = 0;
		unsigned long position;
		while((str == "") || mydictionary->isExist(str,hash,position))
		{
			int ch = getc(pointer);
			if(ch == EOF)
			{
				no_EOF = false;
				break;
			}
			else
			{
				str = str + (char)ch;
				hash = (ch + (hash << 16) + (hash << 6) - hash) % mydictionary->prime;
			}
		}
		print(output,position,prev_size);
		if(!no_EOF)
		{
			break;
		}
		fseek(pointer,-1,SEEK_CUR);
		mydictionary->insert(str);
		/*if(prev_size != mydictionary->size_char())
		{
			prev_size = mydictionary->size_char();
			fputc(58,output);
			fprintf(output,"%u",prev_size);
			fputc(58,output);
		}*/
	}
	fcloseall();
	double_compress(name);
	remove((name + ".dcmp").c_str());
	delete [] mydictionary->array;
	delete [] mydictionary->dictionary;
	delete mydictionary;
	cout << endl << "File compressed successfully" << endl << endl;
}



void uncompress(string name)
{
	if((name.length() - name.find(".cmp")) != 4)
	{
		cout << "Cannot decompress the given file, 'incorrect format'" << endl;
	}
	else
	{
		initial_uncompress(name);
		FILE * pointer;
		pointer = fopen((name + "d").c_str(),"r");
		if (pointer == NULL) 
		{
			cout << "Unable to open file" << endl;
			exit(-1);
		}
		FILE * output;
		output = fopen(("original_" + name.substr(0,name.find(".cmp"))).c_str(),"w");
		if (output == NULL) 
		{
			cout << "Unable to create file" << endl; 
			exit(-1);
		}
		rewind(pointer);
		hashclass * mydictionary;
		unsigned int start = 0;
		unsigned int prev_size;
		unsigned int counter = 0;
		unsigned long to_output = 0;
		string last_str = "";
		
		while(true)
		{
			int ch = getc(pointer);
			if(ch == EOF)
			{
				break;
			}
			else if(ch == 58)
			{
				fscanf(pointer,"%u",&prev_size);
				getc(pointer);
				if(start == 0)
				{
					start = prev_size;
					mydictionary = new hashclass(start);
				}
				if(prev_size != mydictionary->size_char())
				{
					cout << "Error recovering the original file" << endl;
					exit(-1);
				}
			}
			else if(ch == 59)
			{
				string str;
				to_output = to_output/(pow(10,prev_size-counter));
				if(mydictionary->isExist(to_output,str))
				{
					if(last_str != "")
					{
						last_str = last_str + str.substr(0,1);
						mydictionary->insert(last_str);
					}
					//fputs(str.c_str(),output);
					for(unsigned int j = 0;j<str.length();j++)
					{
						fputc((int)str[j],output);
					}
					last_str = str;
					to_output = 0;
					counter = 0;
				}
				else
				{
					if(last_str != "")
					{
						last_str = last_str + last_str.substr(0,1);
						mydictionary->insert(last_str);
					}
					if(mydictionary->isExist(to_output,str))
					{
						//fputs(str.c_str(),output);
						for(unsigned int j = 0;j<str.length();j++)
						{
							fputc((int)str[j],output);
						}
						last_str = str;
						to_output = 0;
						counter = 0;
					}
					else
					{
						cout << "Error recovering the original file" << endl;
						exit(-1);
					}
				}
			}
			else if(ch == 60)
			{
				delete [] mydictionary->array;
				delete [] mydictionary->dictionary;
				delete mydictionary;
				mydictionary = new hashclass(start);
				prev_size = start;
				last_str = "";
				counter = 0;
			}
			else
			{
				if(counter + 1 == prev_size)
				{
					to_output = to_output + (ch-48);
					string str;
					if(mydictionary->isExist(to_output,str))
					{
						if(last_str != "")
						{
							last_str = last_str + str.substr(0,1);
							mydictionary->insert(last_str);
						}
						//fputs(str.c_str(),output);
						for(unsigned int j = 0;j<str.length();j++)
						{
							fputc((int)str[j],output);
						}
						last_str = str;
						to_output = 0;
						counter = 0;
					}
					else
					{
						if(last_str != "")
						{
							last_str = last_str + last_str.substr(0,1);
							mydictionary->insert(last_str);
						}
						if(mydictionary->isExist(to_output,str))
						{
							//fputs(str.c_str(),output);
							for(unsigned int j = 0;j<str.length();j++)
							{
								fputc((int)str[j],output);
							}
							last_str = str;
							to_output = 0;
							counter = 0;
						}
						else
						{
							cout << "Error recovering the original file" << endl;
							exit(-1);
						}
					}
				}
				else
				{
					counter++;
					to_output = to_output + (ch-48)*pow(10,prev_size-counter);
				}	
			}
		}
		fcloseall();
		remove((name + "d").c_str());
		delete [] mydictionary->array;
		delete [] mydictionary->dictionary;
		delete mydictionary;
		cout << endl << "Original file recovered successfully" << endl << endl;
	}
}

