
#include "RandGenerator.h"

int* CRandGenerator::GetElemWithoutDuplication(int from, int to)
{
	std::random_device rand;
	std::mt19937 generator( rand() );

	std::uniform_int_distribution<> dist(from, to);
	int count = to-from+1;

	int* generatedNum = new int[count];
	int generatedNumCount = 0;

	for(int i=0; i<count; ++i)
	{
		int tmp = dist(generator);
		bool duplicated = false;
		
		// 중복 여부 검사
		for(int j=0; j<generatedNumCount; ++j)
		{
			if( generatedNum[j] == tmp )
			{
				duplicated = true;
				break;
			}
		}
		if( duplicated )
		{
			--i;
		}
		else
		{
			generatedNum[generatedNumCount] = tmp;
			++generatedNumCount;
		}
	}

	return generatedNum;
}

float CRandGenerator::GetAnyFloat(float from, float to)
{
	std::random_device rand;
	std::mt19937 generator( rand() );

	std::uniform_real_distribution<> dist(from, to);
	return dist(generator);
}

int CRandGenerator::GetAnyInt(int from, int to)
{
	std::random_device rand;
	std::mt19937 generator( rand() );

	std::uniform_int_distribution<> dist(from, to);
	return dist(generator);
}
