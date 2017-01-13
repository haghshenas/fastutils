
#ifndef __COMMON__
#define __COMMON__

#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <math.h>
#include <stdint.h>
#include <limits>
#include <zlib.h>

using namespace std;

void 				initCommon();
string 				str2Lower(string str);
string 				str2Upper(string str);
void 				revString(string &str, string &revStr);
void 				revComplement(string &str, string &revStr);
void 				revString(string &revStr, char *str);
int 				parseCommandLine (int argc, char *argv[]);
void 				revComplement(string &revStr, char *str);
void 				printHelp();

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

// global in common
char 		_com_revVal[128];
char 		_com_lowVal[128];
char 		_com_upVal[128];

void initCommon()
{
	_com_revVal['a'] = 't';
	_com_revVal['A'] = 'T';
	_com_revVal['c'] = 'g';
	_com_revVal['C'] = 'G';
	_com_revVal['g'] = 'c';
	_com_revVal['G'] = 'C';
	_com_revVal['t'] = 'a';
	_com_revVal['T'] = 'A';
	_com_revVal['n'] = 'n';
	_com_revVal['N'] = 'N';
	_com_revVal['-'] = '-';

	_com_lowVal['a'] = _com_lowVal['A'] = 'a';
	_com_lowVal['c'] = _com_lowVal['C'] = 'c';
	_com_lowVal['g'] = _com_lowVal['G'] = 'g';
	_com_lowVal['t'] = _com_lowVal['T'] = 't';
	_com_lowVal['n'] = _com_lowVal['N'] = 'n';

	_com_upVal['a'] = _com_upVal['A'] = 'A';
	_com_upVal['c'] = _com_upVal['C'] = 'C';
	_com_upVal['g'] = _com_upVal['G'] = 'G';
	_com_upVal['t'] = _com_upVal['T'] = 'T';
	_com_upVal['n'] = _com_upVal['N'] = 'N';
}

string str2Lower(string str)
{
	string lowerStr(str.size(), 'n');
	for(int i=0; i<str.size(); i++)
	{
		lowerStr[i] = _com_lowVal[str[i]];
	}
	return lowerStr;
}

string str2Upper(string str)
{
	string upperStr(str.size(), 'n');
	for(int i=0; i<str.size(); i++)
	{
		upperStr[i] = _com_upVal[str[i]];
	}
	return upperStr;
}

void revString(string &revStr, string &str)
{
	int n = str.size();
	revStr.assign(n, 'N');
	for(int i=0; i<n; i++)
		revStr[i] = str[n-i-1];
}

void revComplement(string &revStr, string &str)
{
	int n = str.size();
	revStr.assign(n, 'N');
	for(int i=0; i<n; i++)
		revStr[i] = _com_revVal[str[n-i-1]];
}

void revString(string &revStr, char *str)
{
	int n = strlen(str);
	revStr.assign(n, 'N');
	for(int i=0; i<n; i++)
		revStr[i] = str[n-i-1];
}

void revComplement(string &revStr, char *str)
{
	int n = strlen(str);
	revStr.assign(n, 'N');
	for(int i=0; i<n; i++)
		revStr[i] = _com_revVal[str[n-i-1]];
}

#endif //__COMMON__