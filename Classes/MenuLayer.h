
#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "GameMode.h"
USING_NS_CC;

/** 
 @brief Layer for menu 
 */
class CMenuLayer : public cocos2d::CCLayer
{
public:

	CREATE_FUNC(CMenuLayer);
	virtual bool init();  

	/** After player is died, Menu layer has to be awaken */
	void Awake();

	/** Let the mode sprite move down */
	void MoveModeSpriteDown(float length);
	void ChangeGameMode(CGameMode::E_GameMode gameMode);

	void OnClickRankButton(CCObject* pSender);
	void OnClickYesButton(CCObject* pSender);
	void OnClickNoButton(CCObject* pSender);
	void OnClickTutorialButton(CCObject* pSender);

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);

private:
	CCMenu*		_mMenu;
	CCSprite*	_mMenuBkgSprite;
	CCSprite*	_mModeSprite;

	CCSprite*	_mThornSprite;

	/** Sprite - You can change the mode by swiping */
	CCSprite*	_mSpriteYouCanSwipe;

	/** If it is start menu or try again menu */
	bool		_mStartMenu;

	/** Don't let the mode sprite move after game start */
	bool		_mGameStart;
	
	CCPoint		_mOldTouch;

};


#endif