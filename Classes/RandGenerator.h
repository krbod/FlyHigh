
#ifndef __RAND_GENERATOR_H__
#define __RAND_GENERATOR_H__

#include <random>

/**
 @brief C++ random engine 을 wrapping 
 @ref http://en.cppreference.com/w/cpp/numeric/random
 */
class CRandGenerator
{
public:
	
	/** Float 값 하나를 랜덤 생성해서 리턴
	 @param from 생성할 값의 시작 범위
	 @param to 생성할 값의 끝 범위
	 @return 랜덤 생성한 float 값
	 */
	static float GetAnyFloat(float from, float to);

	static int GetAnyInt(int from, int to);
		

	
	/** 중복되지 않는 랜덤한 숫자를 가진 배열을 생성
	 @param from 생성할 랜덤 숫자의 시작 범위
	 @param to 생성할 램덤 숫자의 끝 범위
	 @return 생성한 배열을 리턴
	 */
	//static int* GetElemWithoutDuplication(int from, int to);
};


#endif
