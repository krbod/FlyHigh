
#ifndef __INTRO_H__
#define __INTRO_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 @brief Intro scene
 This scene show the logo of company
 */
class CIntroScene : public CCLayer{
	
public:
	CIntroScene() : _mLogoSprite(NULL), _mLogoOpacity(0), _mIsBrighterOpacity(true){}
	
	virtual bool init();  		
	static cocos2d::CCScene* scene();
	
	CREATE_FUNC(CIntroScene);

private:
	/** schedule function
	Draw the logo
	*/
	void _DrawIntro(float dt);

private:
	CCSprite*		_mLogoSprite;
	int				_mLogoOpacity;
	bool			_mIsBrighterOpacity;

};


#endif