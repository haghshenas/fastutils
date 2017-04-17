
#ifndef __COMMON__
#define __COMMON__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <getopt.h>
#include <zlib.h>
#include <limits.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

string str2Lower(string str);
string str2Upper(string str);
void   revString(string &str, string &revStr);
void   revString(string &revStr, char *str);
void   revComplement(string &str, string &revStr);
void   revComplement(string &revStr, char *str);

template <typename T>
T str2type(string str)
{
	T n;
	istringstream sin(str);
	sin >> n;
	return n;
}

template <typename T>
string type2str(T v)
{
	ostringstream sout;
	sout << v;
	return sout.str();
}

#endif //__COMMON__
