
#ifndef __RAND_GENERATOR_H__
#define __RAND_GENERATOR_H__

#include <random>

/**
 @brief C++ random engine �� wrapping 
 @ref http://en.cppreference.com/w/cpp/numeric/random
 */
class CRandGenerator
{
public:
	
	/** Float �� �ϳ��� ���� �����ؼ� ����
	 @param from ������ ���� ���� ����
	 @param to ������ ���� �� ����
	 @return ���� ������ float ��
	 */
	static float GetAnyFloat(float from, float to);

	static int GetAnyInt(int from, int to);
		

	
	/** �ߺ����� �ʴ� ������ ���ڸ� ���� �迭�� ����
	 @param from ������ ���� ������ ���� ����
	 @param to ������ ���� ������ �� ����
	 @return ������ �迭�� ����
	 */
	//static int* GetElemWithoutDuplication(int from, int to);
};


#endif
