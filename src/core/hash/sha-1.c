#include "sha-1.h"

#include <stdlib.h>
#include <inttypes.h>

typedef struct _uint160 {
	uint8_t bitBuff[20];
} uint160_t;

static char *itohex(uint160_t val)
{
	static char symTable[17] = "0123456789abcdef";

	char *res = (char *)malloc(sizeof(char)*41);
	int index = 40;
	uint8_t rmask = 0x0f, lmask = 0xf0, value;

	res[index--] = 0;

	for (int i = 19; i >= 0; i--) {
		value = val.bitBuff[i];
		res[index--] = symTable[value & rmask];
		res[index--] = symTable[(value & lmask) >> 4];
	}

	return res;
}