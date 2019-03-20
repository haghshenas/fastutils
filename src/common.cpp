#include "common.hpp"

// global in common
char _com_lowVal[128] = {
   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
  64, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',  91,  92,  93,  94,  95,
  96, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 123, 124, 125, 126, 127
};

char _com_upVal[128] = {
   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
  64, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',  91,  92,  93,  94,  95,
  96, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 123, 124, 125, 126, 127
};

char _com_revVal[128] = {
   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
  64, 'T', 'B', 'G', 'D', 'E', 'F', 'C', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'A', 'U', 'V', 'W', 'X', 'Y', 'Z',  91,  92,  93,  94,  95,
  96, 't', 'b', 'g', 'd', 'e', 'f', 'c', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 'a', 'u', 'v', 'w', 'x', 'y', 'z', 123, 124, 125, 126, 127
};

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
    string upperStr(str.size(), 'N');
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

void revString(string &revStr, char *str)
{
    int n = strlen(str);
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

void revComplement(string &revStr, char *str)
{
    int n = strlen(str);
    revStr.assign(n, 'N');
    for(int i=0; i<n; i++)
        revStr[i] = _com_revVal[str[n-i-1]];
}