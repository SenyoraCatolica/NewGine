#include "GlobalFunctions.h"

unsigned int GenerateUUID()
{
	unsigned int ret;

	const unsigned int bytes = sizeof(long long);
	char myString[bytes];

	for (int n = 0; n < bytes; n++)
	{
		myString[n] = rand() % 255;
	}

	memcpy(&ret, myString, bytes);
	return ret;
}