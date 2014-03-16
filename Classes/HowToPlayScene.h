
#ifndef __HOW_TO_PLAY_SCENE_H__
#define __HOW_TO_PLAY_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;


class CHowToPlayScene : public CCLayer
{
public:
	virtual bool init();  		
	static cocos2d::CCScene* scene();

	CREATE_FUNC(CHowToPlayScene);

	void OnClickDontShowButton(CCObject* pSender);
	
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);
		
private:
	CCSprite*	_mHelpSprite;

	CCPoint		_mOldTouch;

	const char*	_mPath[4];
	int			_mCurrentIndex;
};


#endif

