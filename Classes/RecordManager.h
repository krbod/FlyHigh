#ifndef __RECORD_MANAGER_H__
#define __RECORD_MANAGER_H__

#include "cocos2d.h"
#include "GameMode.h"

USING_NS_CC;

const int RECORD_RATIO = 10;

/**
 @brief Managing the record 
 */
class CRecordManager
{
public:
	void init(CCLayer* scene);

	/** Get current height divided by RECORD_RATIO 
		
	*/
	inline int GetCurrentHeight()
	{
		return _mCurHeight / RECORD_RATIO;
	}
	inline void SetCurrentHeight(int height)
	{
		_mCurHeight = height;
	}
	
	/** Save the best height to user default */
	void SaveBestRecord();

	void UpdateRecord(float height);

	void DisplayCurrentBestRecord();

private:
	
	int			_mBestHeight[kModeCount];

	int			_mCurHeight;
	int			_mCurrentBestHeight;

	bool		_mBestUpdated;
};

#endif