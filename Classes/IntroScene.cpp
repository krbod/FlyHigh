
#include "IntroScene.h"
#include "SimpleAudioEngine.h"
#include "MainGameScene.h"
#include "AppMacros.h"

using namespace CocosDenshion;

const char*		IMAGE_SPRITE_LOGO_BKG = "res/logo.png";
const int		OPACITY_SPEED = 2;

bool CIntroScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	// Make the logo sprite
	_mLogoSprite = CCSprite::create(IMAGE_SPRITE_LOGO_BKG);
	_mLogoSprite->setAnchorPoint(ccp(0, 0));
	_mLogoSprite->setOpacity( 0 );
	this->addChild( _mLogoSprite );

	// Preload the effect 
	SimpleAudioEngine::sharedEngine()->preloadEffect(SOUND_PRE_LOAD_FLY);

	// Set the schedule for drawing logo
	this->schedule(schedule_selector(CIntroScene::_DrawIntro), 1.0f/60);
	
	return true;
}

cocos2d::CCScene* CIntroScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	CIntroScene *layer = CIntroScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void CIntroScene::_DrawIntro( float dt )
{
	if( _mIsBrighterOpacity == false && _mLogoOpacity == 0 ){
		this->unschedule(schedule_selector(CIntroScene::_DrawIntro) );

		CCDirector::sharedDirector()->replaceScene( MainGameScene::scene() );
		return;
	}
	if( _mLogoOpacity == 254 )
		_mIsBrighterOpacity = false;

	if( _mIsBrighterOpacity )
		_mLogoOpacity += OPACITY_SPEED;
	else
		_mLogoOpacity -= OPACITY_SPEED;

	_mLogoSprite->setOpacity( _mLogoOpacity );
}