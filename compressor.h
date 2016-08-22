#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <iostream>
using namespace std;


void print(FILE * pointer,unsigned long to_print,unsigned int size);

void double_compress(string name);

void initial_uncompress(string name);

void compress(string name);

void uncompress(string name);

#endif