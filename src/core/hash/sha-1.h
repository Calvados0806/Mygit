#ifndef SHA_1_H
#define SHA_1_H

#include "../utils.h"

#include <inttypes.h>

MYGIT_HEADER_BEGIN

typedef struct _uint160 {
	uint32_t bitBuff[5];
} uint160_t;

char *itohex(uint160_t val);

uint160_t GetHashCode(const char *const data);

MYGIT_HEADER_END

#endif // SHA_1_H