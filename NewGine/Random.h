#ifndef __RANDOM_H__
#define __RANDOM_H__
#include <stdint.h>
class Random
{
public:
	float GenerateRandomFloat();
	int GenerateRandomInt(int min, int max);
};
#endif // !__RANDOM_H__
