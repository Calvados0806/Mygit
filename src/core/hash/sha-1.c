#include "sha-1.h"

#include <stdlib.h>
#include <inttypes.h>


#define LeftRot(x, n) (((x) << (n)) | ((x) & (~0 << sizeof(x)*8-(n))) >> sizeof(x)*8-(n))

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
	typedef uint64_t bitbuff512[8];

	uint32_t hashes[] = {
		0x67452301,
		0xEFCDAB89,
		0x98BADCFE,
		0x10325476,
		0xC3D2E1F0
	};
	const uint64_t len = (uint64_t)strlen(data);
	const uint64_t bitLen = len * 8;
	const size_t bytesToFill = 64 - len % 64;
	const size_t newLen = len + bytesToFill;

	char *extendBuff = (char *)malloc(newLen);
	memcpy(extendBuff, data, len);

	memset(extendBuff + len, 0, bytesToFill);
	extendBuff[len] |= '\x80';
	memcpy(extendBuff + newLen - 8, &bitLen, 8);

	uint32_t W[80];
	uint32_t partHashes[5];
	uint32_t f, k;

	for (bitbuff512 *iter = (bitbuff512 *)extendBuff; iter < (bitbuff512 *)(extendBuff + newLen); iter++) {
		memcpy(&W, iter, sizeof(bitbuff512));

		for (int i = 16; i < 80; i++) {
			uint32_t val = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
			W[i] = LeftRot(val, 1);
		}

		memcpy(partHashes, hashes, sizeof hashes);

		for (int i = 0; i < 80; i++) {
			if (i < 20) {
				f = partHashes[1] & partHashes[2] | ~partHashes[1] & partHashes[3];
				k = 0x5A827999;
			}
			else if (i < 40) {
				f = partHashes[1] ^ partHashes[2] ^ partHashes[3];
				k = 0x6ED9EBA1;
			}
			else if (i < 60) {
				f = partHashes[1] & partHashes[2] | partHashes[1] & partHashes[3] | partHashes[2] & partHashes[3];
				k = 0x8F1BBCDC;
			}
			else {
				f = partHashes[1] ^ partHashes[2] ^ partHashes[3];
				k = 0xCA62C1D6;
			}

			uint32_t temp = (LeftRot(partHashes[0], 5) + (uint64_t)f + partHashes[4] + k + W[i]) % 0xffffffff;
			partHashes[4] = partHashes[3];
			partHashes[3] = partHashes[2];
			partHashes[2] = LeftRot(partHashes[1], 30);
			partHashes[1] = partHashes[0];
			partHashes[0] = temp;
		}

		hashes[0] = ((uint64_t)hashes[0] + partHashes[0]) % 0xffffffff;
		hashes[1] = ((uint64_t)hashes[1] + partHashes[1]) % 0xffffffff;
		hashes[2] = ((uint64_t)hashes[2] + partHashes[2]) % 0xffffffff;
		hashes[3] = ((uint64_t)hashes[3] + partHashes[3]) % 0xffffffff;
		hashes[4] = ((uint64_t)hashes[4] + partHashes[4]) % 0xffffffff;
	}

	uint160_t retVal;
	memcpy(&retVal.bitBuff, hashes, sizeof hashes);

	free(extendBuff);
	return itohex(retVal);
}