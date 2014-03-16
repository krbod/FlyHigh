
#include "RecordLayer.h"
#include "GameMode.h"
#include "AppMacros.h"

bool CRecordLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int _mBestHeight = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_LIMITED_BEST_RECORD);

	// Init best record
	_mBestRecord = CCLabelTTF::create(
		CCString::createWithFormat("BEST:%dM", _mBestHeight/100)->getCString(), 
		FONT_FAMILY,
		winSize.height/25);
	_mBestRecord->setAnchorPoint(ccp(0.0f, 0.0f));
	_mBestRecord->setColor(ccc3(255, 40, 40));
	_mBestRecord->setPosition( ccp(winSize.width * 0.5 / 10, winSize.height * 9 / 10) );
	this->addChild(_mBestRecord);

	// Init current record
	_mCurRecord = CCLabelTTF::create("", FONT_FAMILY, winSize.height/25);
	_mCurRecord->setAnchorPoint(ccp(0.0f, 0.0f));
	_mCurRecord->setColor(ccc3(255, 40, 40));
	_mCurRecord->setPosition( ccp(winSize.width * 0.5 / 10, winSize.height * 9 / 10 + 100) );
	this->addChild(_mCurRecord);

	return true;
}

void CRecordLayer::SetBestRecord()
{
	int bestHeight;
	switch( CGameMode::GetInstance()->GetGameMode() )
	{
	case CGameMode::kModeLimited:
		bestHeight = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_LIMITED_BEST_RECORD);
		break;

	case CGameMode::kModeUnLimited:
		bestHeight = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_UNLIMITED_BEST_RECORD);
		break;
	}

	_mBestRecord->setString(
		CCString::createWithFormat("BEST:%dM", bestHeight/100)->getCString());
	
}
