// Copyright Jay Conrod 2010.
// http://jayconrod.com
// jayconrod@gmail.com
//
// You are free to modify and distribute this code as long as this
// copyright notice is maintained.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
#define _CRT_NO_VA_START_VALIDATION
#include "Utilities.h"

#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstdarg>


using namespace std;

string format(const string &fmt, ...) {
	va_list args;

	va_start(args, fmt);
	char *buffer = NULL;
	int size = vsnprintf(buffer, 0, fmt.c_str(), args);
	va_end(args);
	buffer = new char[size + 1];

	va_start(args, fmt);
	vsnprintf(buffer, size + 1, fmt.c_str(), args);
	va_end(args);
	string str(buffer);
	delete[] buffer;

	va_end(args);
	return str;
}

