
#include "RecordManager.h"
#include "RecordLayer.h"
#include "AppMacros.h"

void CRecordManager::init(CCLayer* layer)
{
	// Get best records.
	_mBestHeight[kModeUnLimited] = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_UNLIMITED_BEST_RECORD);
	_mBestHeight[kModeLimited] = CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DEFAULT_LIMITED_BEST_RECORD);

	_mBestUpdated = false;
	_mCurrentBestHeight = 0;
}

void CRecordManager::UpdateRecord(float height)
{
	_mCurHeight += height;	
	if( _mCurHeight < 0 )
		_mCurHeight = 0;

	//int cm = GetCurrentHeight() % 10;
	int m = GetCurrentHeight() / 10;

	CRecordLayer* layer = static_cast<CRecordLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_RECORD));
	layer->SetCurrentRecord(m);

	// when player is died, The best height of this mode and current height of this game is displayed.
	// Current height record is set when player is died. 
	// but it isn't the highest record of current game. because player is died after dropping few meters.
	// so we need to have the another var that store the highest height of this game.
	if( _mCurHeight > _mCurrentBestHeight )
		_mCurrentBestHeight = _mCurHeight;
	
	E_GameMode mode = CGameMode::GetInstance()->GetGameMode();
	if( _mCurHeight > _mBestHeight[mode] )
	{
		_mBestHeight[mode] = _mCurHeight;
		layer->SetBestRecord(m);

		_mBestUpdated = true;
	}

}


void CRecordManager::SaveBestRecord()
{
	if(!_mBestUpdated)
		return;

	switch( CGameMode::GetInstance()->GetGameMode() )
	{
	case kModeLimited:
		CCUserDefault::sharedUserDefault()->setIntegerForKey(
			USER_DEFAULT_LIMITED_BEST_RECORD, 
			_mBestHeight[CGameMode::GetInstance()->GetGameMode()]);
		break;

	case kModeUnLimited:
		CCUserDefault::sharedUserDefault()->setIntegerForKey(
			USER_DEFAULT_UNLIMITED_BEST_RECORD, 
			_mBestHeight[CGameMode::GetInstance()->GetGameMode()]);
		break;
	}
	
	_mBestUpdated = false;
}

void CRecordManager::DisplayCurrentBestRecord()
{
	CRecordLayer* layer = static_cast<CRecordLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_RECORD));
	layer->SetCurrentRecord(_mCurrentBestHeight/100);

	_mCurrentBestHeight = 0;
}



