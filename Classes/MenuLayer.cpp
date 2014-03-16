
#include "MenuLayer.h"
#include "MainGameScene.h"
#include "RecordLayer.h"
#include "RankingScene.h"
#include "HowToPlayScene.h"
#include "AppMacros.h"

const char* TRY_AGAIN_SPRITE_PATH = "res/tryAgain.png";
const char* MAIN_MENU_SPRITE_PATH = "res/mainMenu.png";
const char* YOU_CAN_SWIPE_SPRITE_PATH = "res/youcanswipe.png";

const char* BTN_ONLINE_SPRITE_PATH = "res/rank.png";
const char* BTN_ONLINE_CLICKED_SPRITE_PATH = "res/rank_clicked.png";
const char* BTN_YES_SPRITE_PATH = "res/yes.png";
const char* BTN_YES_CLICKED_SPRITE_PATH = "res/yes_clicked.png";
const char* BTN_NO_SPRITE_PATH = "res/no.png";
const char* BTN_NO_CLICKED_SPRITE_PATH = "res/no_clicked.png";
const char* BTN_TUTORIAL_SPRITE_PATH = "res/tutorial.png";
const char* BTN_TUTORIAL_CLICKED_SPRITE_PATH = "res/tutorial_clicked.png";

bool CMenuLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	this->setTouchEnabled( true );

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// Draw the menu's background image
	_mMenuBkgSprite = CCSprite::create(MAIN_MENU_SPRITE_PATH);
	_mMenuBkgSprite->setPosition(ccp(winSize.width/2, winSize.height*4.7/10));
	this->addChild(_mMenuBkgSprite);

	_mModeSprite = CCSprite::create(GAME_MODE_SPRITE_PATH);
	_mModeSprite->setPosition(ccp(0, winSize.height/2));
	_mModeSprite->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(_mModeSprite);

	_mThornSprite = CCSprite::create(THORN_SPRITE_PATH);
	_mThornSprite->setPosition(ccp(winSize.width, 0));
	_mThornSprite->setAnchorPoint(ccp(0.0f, 0.0f));
	this->addChild(_mThornSprite);  
	
	_mSpriteYouCanSwipe = CCSprite::create(YOU_CAN_SWIPE_SPRITE_PATH);
	_mSpriteYouCanSwipe->setPosition(ccp(winSize.width/2, winSize.height*2.3/10));
	this->addChild(_mSpriteYouCanSwipe);
	
	// Make the YES button
	CCMenuItemImage* btnYesSprite = CCMenuItemImage::create(
		BTN_YES_SPRITE_PATH,
		BTN_YES_CLICKED_SPRITE_PATH,
		this, 
		menu_selector(CMenuLayer::OnClickYesButton) );

	btnYesSprite->setPosition(ccp(winSize.width * 2.9 / 10, winSize.height * 4.1 / 10) );
	
	// Make the NO button
	CCMenuItemImage* btnNoSprite = CCMenuItemImage::create(
		BTN_NO_SPRITE_PATH,
		BTN_NO_CLICKED_SPRITE_PATH,
		this, 
		menu_selector(CMenuLayer::OnClickNoButton) );

	btnNoSprite->setPosition(ccp(winSize.width * 5 / 10, winSize.height * 4.1 / 10) );
	
	// Make the ONLINE button
	CCMenuItemImage* btnRankSprite = CCMenuItemImage::create(
		BTN_ONLINE_SPRITE_PATH,
		BTN_ONLINE_CLICKED_SPRITE_PATH,
		this, 
		menu_selector(CMenuLayer::OnClickRankButton) );

	btnRankSprite->setPosition(ccp(winSize.width * 7.1 / 10, winSize.height * 4.1 / 10) );

	// Make the YES button
	CCMenuItemImage* btnTutorialSprite = CCMenuItemImage::create(
		BTN_TUTORIAL_SPRITE_PATH,
		BTN_TUTORIAL_CLICKED_SPRITE_PATH,
		this, 
		menu_selector(CMenuLayer::OnClickTutorialButton) );

	btnTutorialSprite->setPosition(ccp(winSize.width * 8.5 / 10, winSize.height * 9 / 10) );

	_mMenu = CCMenu::create(btnYesSprite, btnNoSprite, btnRankSprite, btnTutorialSprite, NULL);
	_mMenu->setPosition(ccp(0, 0));
	this->addChild(_mMenu, 10);

	_mStartMenu = true;
	_mGameStart = false;

	return true;
}

void CMenuLayer::OnClickYesButton(CCObject* pSender)
{
	MainGameScene* mainLayer = static_cast<MainGameScene*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(0));
	if( _mStartMenu )
		mainLayer->SetMainLoop();
	else
		mainLayer->Reset();

	_mMenuBkgSprite->setVisible(false);
	_mMenu->setVisible(false);

	_mGameStart = true;

	// change image to "try again" one
	if( _mStartMenu )
	{
		_mMenuBkgSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(TRY_AGAIN_SPRITE_PATH));
		_mStartMenu = false;
	}	

	_mSpriteYouCanSwipe->setVisible(false);

}

void CMenuLayer::OnClickNoButton(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

void CMenuLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	if( _mGameStart )
		return;
	
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();

		_mOldTouch = touchPoint;
	}
}

void CMenuLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	if( _mGameStart )
		return;
	
	if( static_cast<MainGameScene*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_MAIN_GAME))->GetIsDragging() )
		return;

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

void CMenuLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	if( _mGameStart )
		return;

	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();
		
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 		if( _mModeSprite->getPositionX() <= winSize.width/2 * -1)
		{
			ChangeGameMode(CGameMode::kModeLimited);		
		}
		else
		{
			ChangeGameMode(CGameMode::kModeUnLimited);		
		}

	}
}

void CMenuLayer::MoveModeSpriteDown(float length)
{
	CCPoint curPoint = _mModeSprite->getPosition();
	curPoint.y += length;

	_mModeSprite->setPosition(curPoint);
}

void CMenuLayer::Awake()
{
	// Show the menu
	_mMenuBkgSprite->setVisible(true);
	_mMenu->setVisible(true);

	// Reset the mode sprite's position
	CCPoint curPoint = _mModeSprite->getPosition();
	curPoint.y = CCDirector::sharedDirector()->getWinSize().height/2;
	_mModeSprite->setPosition(curPoint);

	// Set the gamestart var to false so that mode sprite can move 
	_mGameStart = false;

	_mSpriteYouCanSwipe->setVisible(true);
}

void CMenuLayer::OnClickRankButton(CCObject* pSender)
{
	CCDirector::sharedDirector()->pushScene( CRankingScene::scene() );
}

void CMenuLayer::OnClickTutorialButton(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene( CHowToPlayScene::scene() );
}

void CMenuLayer::ChangeGameMode(CGameMode::E_GameMode gameMode)
{
	CCActionInterval* moveTo, *moveToForThorn;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	switch( gameMode )
	{
	case CGameMode::kModeUnLimited:
		// Action for game mode sprite
		moveTo = CCMoveTo::create(0.3f, ccp(0, winSize.height/2));

		// Action for thron sprite
		moveToForThorn = CCMoveTo::create(0.3f, ccp(winSize.width, 0));
		break;

	case CGameMode::kModeLimited:

		// Action for game mode sprite
		moveTo = CCMoveTo::create(0.3f, ccp(winSize.width*-1, winSize.height/2));

		// Action for thron sprite
		moveToForThorn = CCMoveTo::create(0.3f, ccp(0, 0));
		break;
	}

	CCActionInterval* moveEase = CCEaseExponentialIn::create(moveTo);
	_mModeSprite->runAction(moveEase);

	CCActionInterval* moveEaseForThorn = CCEaseExponentialIn::create(moveToForThorn);
	_mThornSprite->runAction(moveEaseForThorn);
	
	CRecordLayer* recordLayer = static_cast<CRecordLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_RECORD));

	// Dont change the current record when just clicking.
	// current record has to be modified when game mode is changed.
	if( gameMode != CGameMode::GetInstance()->GetGameMode() )
		recordLayer->SetCurrentRecord(0);

	// After changing the game mode, change the best record of changed mode.
	CGameMode::GetInstance()->SetGameMode(gameMode);
	recordLayer->SetBestRecord();

}
