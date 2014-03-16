
#ifndef __RANKING_MODE_LAYER_H__
#define __RANKING_MODE_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

/** 
 @brief Layer for menu 
 */
class CRankingModeLayer : public cocos2d::CCLayer
{
public:

	CREATE_FUNC(CRankingModeLayer);
	virtual bool init();  

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);

private:
	CCSprite*	_mModeSprite;
	CCSprite*	_mThornSprite;

	CCSprite*	_mSwipeSprite;
	CCSprite*	_mSwipeOppSprite;
			
	CCPoint		_mOldTouch;

};


#endif