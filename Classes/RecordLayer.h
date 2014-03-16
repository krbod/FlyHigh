
#ifndef __RECORD_LAYER_H__
#define __RECORD_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

/** 
 @brief Layer to display the record 
 */
class CRecordLayer : public cocos2d::CCLayer
{
public:

	CREATE_FUNC(CRecordLayer);
	virtual bool init();  

	void SetBestRecord();

	/** Display the best record */
	inline void SetBestRecord(int record)
	{
		_mBestRecord->setString(
			CCString::createWithFormat("BEST:%dM", record)->getCString());
	}

	/** Display the current record */
	inline void SetCurrentRecord(int record)
	{
		_mCurRecord->setString(
			CCString::createWithFormat("NOW:%dM", record)->getCString());
	}

private:
	CCLabelTTF* _mBestRecord;
	CCLabelTTF* _mCurRecord;

};


#endif