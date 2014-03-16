
#include "RankingModeLayer.h"
#include "MainGameScene.h"
#include "RecordLayer.h"
#include "RankingScene.h"
#include "MenuLayer.h"
#include "AppMacros.h"

const char* SWIPE_SPRITE_PATH = "res/swipe.png";
const char* SWIPE_OPP_SPRITE_PATH = "res/swipe_opp.png";

bool CRankingModeLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	this->setTouchEnabled( true );

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	_mModeSprite = CCSprite::create(GAME_MODE_SPRITE_PATH);
	_mModeSprite->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(_mModeSprite);

	_mThornSprite = CCSprite::create(THORN_SPRITE_PATH);
	_mThornSprite->setAnchorPoint(ccp(0.0f, 0.0f));
	this->addChild(_mThornSprite); 

	_mSwipeSprite = CCSprite::create(SWIPE_SPRITE_PATH);
	_mSwipeSprite->setPosition(ccp(winSize.width, winSize.height * 0.4 / 10));
	_mSwipeSprite->setAnchorPoint(ccp(1.0f, 0.0f));
	this->addChild(_mSwipeSprite);

	_mSwipeOppSprite = CCSprite::create(SWIPE_OPP_SPRITE_PATH);
	_mSwipeOppSprite->setPosition(ccp(0, winSize.height * 0.4 / 10));
	_mSwipeOppSprite->setAnchorPoint(ccp(0.0f, 0.0f));
	this->addChild(_mSwipeOppSprite);

	switch( CGameMode::GetInstance()->GetGameMode() )
	{
	case CGameMode::kModeUnLimited:
		_mSwipeSprite->setVisible(true);
		_mSwipeOppSprite->setVisible(false);

		_mModeSprite->setPosition(ccp(0, winSize.height/2));
		_mThornSprite->setPosition(ccp(winSize.width, 0));
		break;

	case CGameMode::kModeLimited:
		_mSwipeSprite->setVisible(false);
		_mSwipeOppSprite->setVisible(true);

		_mModeSprite->setPosition(ccp(winSize.width*-1, winSize.height/2));
		_mThornSprite->setPosition(ccp(0, 0));
		break;

	}

	return true;
}


void CRankingModeLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();

		_mOldTouch = touchPoint;

		_mSwipeSprite->setVisible(false);
		_mSwipeOppSprite->setVisible(false);
	}
}

void CRankingModeLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();

		float distance = touchPoint.x - _mOldTouch.x;

		float x = _mModeSprite->getPositionX();
		_mModeSprite->setPositionX(x + distance);


		x = _mThornSprite->getPositionX();
		_mThornSprite->setPositionX(x + distance);

		_mOldTouch = touchPoint;
	}
}

void CRankingModeLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();
		
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 		if( _mModeSprite->getPositionX() <= winSize.width/2 * -1)
		{
			// Action for game mode sprite
			CCActionInterval* moveTo = CCMoveTo::create(0.3f, ccp(winSize.width*-1, winSize.height/2));
			CCActionInterval* moveEase = CCEaseExponentialIn::create(moveTo);
			_mModeSprite->runAction(moveEase);

			// Action for thron sprite
			CCActionInterval* moveToForThorn = CCMoveTo::create(0.3f, ccp(0, 0));
			CCActionInterval* moveEaseForThorn = CCEaseExponentialIn::create(moveToForThorn);
			_mThornSprite->runAction(moveEaseForThorn);

			CRankingScene *rankingScene = static_cast<CRankingScene*>(this->getParent());
			rankingScene->GetMyRank(CGameMode::kModeLimited);
			rankingScene->DisplayMyRecord(CGameMode::kModeLimited);

			rankingScene->GetLimitedTop10();

			_mSwipeSprite->setVisible(false);
			_mSwipeOppSprite->setVisible(true);

			CCLog(("limited"));
		}
		else
		{
			// Action for game mode sprite
			CCActionInterval* moveTo = CCMoveTo::create(0.3f, ccp(0, winSize.height/2));
			CCActionInterval* moveEase = CCEaseExponentialIn::create(moveTo);
			_mModeSprite->runAction(moveEase);

			// Action for thron sprite
			CCActionInterval* moveToForThorn = CCMoveTo::create(0.3f, ccp(winSize.width, 0));
			CCActionInterval* moveEaseForThorn = CCEaseExponentialIn::create(moveToForThorn);
			_mThornSprite->runAction(moveEaseForThorn);

			CRankingScene *rankingScene = static_cast<CRankingScene*>(this->getParent());
			rankingScene->GetMyRank(CGameMode::kModeUnLimited);
			rankingScene->DisplayMyRecord(CGameMode::kModeUnLimited);

			rankingScene->GetUnlimitedTop10();

			_mSwipeSprite->setVisible(true);
			_mSwipeOppSprite->setVisible(false);

			CCLog(("unlimited"));
		}

	}
}
