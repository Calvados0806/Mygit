#include "sha-1.h"

#include <stdlib.h>
#include <inttypes.h>


#define LeftRot(x, n) ((x) << (n)) | ((x) & (~0 << sizeof(x)*8-(n))) >> sizeof(x)*8-(n)

typedef struct _uint160 {
	uint32_t bitBuff[5];
} uint160_t;

static char *itohex(uint160_t val)
{
	static char symTable[17] = "0123456789abcdef";
	static uint32_t masks[8] = {
		0x0000000f, 0x000000f0,
		0x00000f00, 0x0000f000,
		0x000f0000, 0x00f00000,
		0x0f000000, 0xf0000000
	};

	char *res = (char *)malloc(sizeof(char)*41);
	int index = 40;

	res[index--] = 0;

	for (int i = 4; i >= 0; i--)
		for (int j = 0; j < 8; j++)
			res[index--] = symTable[(val.bitBuff[i] & masks[j]) >> 4*j];

	return res;
}

char *GetHashCode(const char *const data)
{
	return NULL;
}