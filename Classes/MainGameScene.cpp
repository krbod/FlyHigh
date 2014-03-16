#include "MainGameScene.h"
#include "AppMacros.h"

#include "SimpleAudioEngine.h"

#include "GameMode.h"

using namespace CocosDenshion;

/** When character fly over below value, then new clouds will be made */
const int DELAY_DISTANCE_FOR_MAKING_CLOUD = 100;

/** The limit character can move down */
const int BOTTOM_LIMIT = -2000;

/** Character's sprite doesn't fly any more from Window's height * FLYING_LINE 
	While character's body fly over the flying line, but sprite stop on flying line */
const float FLYING_LINE = 0.98;

/** Box2D constant value */
const int BOX2D_VELOCITY_ITERATIONS = 8;
const int BOX2D_POSITION_ITERATIONS = 3;

const char*		IMAGE_SPRITE_PLAYER = "res/player.png";
const int		DEFAULT_IMAGE_SIZE = 256;

const int		GAME_MODE_LIMITED = 0;
const int		GAME_MODE_UNLIMITED = 1;

CMenuLayer* MainGameScene::_mMenuLayer = 0;
CRecordLayer* MainGameScene::_mRecordLayer = 0;

CCScene* MainGameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	MainGameScene *layer = MainGameScene::create();
	scene->addChild(layer, 0, TAG_LAYER_MAIN_GAME);	

	// Initialize the menu, record layer.
	_mMenuLayer = CMenuLayer::create();
	scene->addChild(_mMenuLayer, Z_ORDER_TRY_AGAIN_LAYER, TAG_LAYER_MENU);

	_mRecordLayer = CRecordLayer::create();
	scene->addChild(_mRecordLayer, Z_ORDER_RECORD_LAYER, TAG_LAYER_RECORD);
		
    return scene;
}

MainGameScene::~MainGameScene()
{
	delete _mWorld;
	_mWorld = NULL;
}

// on "init" you need to initialize your instance
bool MainGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	this->setTouchEnabled( true );

	_mWinSize = CCDirector::sharedDirector()->getWinSize();
	
	// Draw background image
	CCSprite* background = CCSprite::create("res/bkg.png");
	background->setAnchorPoint(ccp(0, 0));
	this->addChild(background, 0);
	
	// Read the best record from user default and draw it.
	_mRecordManager.init(this);
	_mRecordManager.SetCurrentHeight(static_cast<int>(_mWinSize.height * FLYING_LINE));
	
	// Create the batch node for character and character's wing images
	_mPlayerBatchNode = CCSpriteBatchNode::create(IMAGE_SPRITE_PLAYER);
	this->addChild(_mPlayerBatchNode);

	// Prepare for sprite "how to play"
	_mSpriteHowToPlay = CCSprite::create("res/howtoplay.png");
	_mSpriteHowToPlay->setAnchorPoint(ccp(0, 0));
	this->addChild(_mSpriteHowToPlay);
	
	// Prepare for draw node to draw the line between character and touch point
	_mLine = CCDrawNode::create();
	this->addChild(_mLine);

	_mOldDiff = 0.0f;
	_mAccumulatedDiff = 0.0f;
		
	// Initialize the box2d world
	CreateBox2dWorld();
		
	// Make the player's body 
	MakePlayerBody();

	_mPlayerDie = false;

	CGameMode::GetInstance()->SetGameMode(CGameMode::kModeUnLimited);

	return true;
}

bool MainGameScene::CreateBox2dWorld()
{
	// Create world
	// Make the gravity -100 in order to make the character drop rapidly;
	b2Vec2 gravity = b2Vec2(0.0f, -100.f);

	_mWorld = new b2World(gravity);
	_mWorld->SetAllowSleeping(true);
	_mWorld->SetContinuousPhysics( true );

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	b2Body* groundBody = _mWorld->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	// Set the edge. Do not set the top edge to let the character fly over the top
	
	// Make the left and right edge 100 times by original
	// When character fly over the flying line, character shouldn't cross the left, right edge.
	// Left
	groundEdge.Set(b2Vec2(0, -1000), b2Vec2(0, _mWinSize.height/PTM_RATIO * 100) );
	groundBody->CreateFixture(&boxShapeDef);

	// Right
	groundEdge.Set(b2Vec2(_mWinSize.width/PTM_RATIO, -1000), b2Vec2(_mWinSize.width/PTM_RATIO, _mWinSize.height/PTM_RATIO * 100) );
	groundBody->CreateFixture(&boxShapeDef);

	return true;
}

void MainGameScene::GameLoop(float dt)
{	
	CCSprite* spriteData = (CCSprite*)_mPlayerBody->GetUserData();

	if( _mOnDrag )
	{		
		_mLine->clear();
		_mLine->drawSegment(_mDragTouch, spriteData->getPosition(), 3.0,ccc4f(0.84f, 0.7f, 0.0f, 1.0));
	}
	else
	{
		_mLine->clear();
	}
	
	// Check if player has been died.
	if(CGameMode::GetInstance()->GetGameMode() == CGameMode::kModeLimited)
	{
		if( spriteData->getPositionY() - spriteData->getContentSize().height/2 < 0 )
		{
			_SetPlayerDie();
		}
	}
	else
	{
		if( spriteData->getPositionY() - spriteData->getContentSize().height/2 < BOTTOM_LIMIT )
		{
			_SetPlayerDie();
		}
	}
	
	// Take one step in physics space
	_mWorld->Step(dt, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);

	// Compute the difference between flying line and player's body
	float diff = _mPlayerBody->GetPosition().y * PTM_RATIO - _mWinSize.height * FLYING_LINE;

	// when character is below the flying line
	if( diff < 0 )
	{
		// Make the sprite's position same with body's position.
		spriteData->setPosition( ccp(_mPlayerBody->GetPosition().x * PTM_RATIO, _mPlayerBody->GetPosition().y * PTM_RATIO ));
		spriteData->setRotation( -1 * CC_RADIANS_TO_DEGREES( _mPlayerBody->GetAngle() ));

		if( !_mPlayerDie )
			_mRecordManager.UpdateRecord( diff - _mOldDiff );

		_mAccumulatedDiff = 0.0f;
	}
	// when character is over the flying line
	else
	{	
		// Character can fly just with dragging. It has to be blocked.
		if( !_mOnDrag )
		{
			_mRecordManager.UpdateRecord( diff - _mOldDiff );

			// Make new clouds
			if( _mAccumulatedDiff > DELAY_DISTANCE_FOR_MAKING_CLOUD)
			{
				_mCloudManager.MakeCloud(_mRecordManager.GetCurrentHeight() / 10);
				_mAccumulatedDiff = 0.0f;
			}
		
			// Move existing clouds to down
			_mCloudManager.MoveDownAllClouds( _mOldDiff - diff );

			// Move the "how to play" sprite
			MoveHowToPlaySpriteDown( _mOldDiff - diff );
		}
		
		// Set the sprite's X position same with body's X position.
		// Fix the sprite's Y position to window's height * STOP_LINE
		spriteData->setPosition(ccp(
			_mPlayerBody->GetPosition().x * PTM_RATIO, 
			spriteData->getPositionY()));
		
		// When the player's body gets the limit, 
		// Set the Y-transform to the window's height * STOP_LINE
		if( _mOldDiff > diff )
		{
			b2Vec2 p;
			p.x = _mPlayerBody->GetTransform().p.x;
			p.y = (_mWinSize.height * FLYING_LINE) / PTM_RATIO;
			_mPlayerBody->SetTransform( p, 0.0);

			_mOldDiff = 0;
			return;

		}

	}
	_mAccumulatedDiff += diff - _mOldDiff;
	_mOldDiff = diff;

}

void MainGameScene::MakePlayerBody()
{
	_mOnDrag = false;

	// Create the tmp body 
	b2BodyDef bodyDef1;
	bodyDef1.type = b2_staticBody;
	bodyDef1.position.Set(_mWinSize.width/PTM_RATIO, _mWinSize.height/PTM_RATIO);
	bodyDef1.userData = NULL;

	_mJointBody = _mWorld->CreateBody(&bodyDef1);

	b2PolygonShape staticBox;
	staticBox.SetAsBox(0.0f, 0.0f);

	b2FixtureDef fixtureDef1;
	fixtureDef1.shape = &staticBox;
	fixtureDef1.density = 1.0f;

	_mJointBody->CreateFixture( &fixtureDef1);

	// Set the player's body
	CCSprite* playerSprite = CCSprite::createWithTexture( 
		_mPlayerBatchNode->getTexture(),
		CCRectMake( 0, 0, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE)
		);
	playerSprite->setAnchorPoint(ccp(0.5, 0.5));
	playerSprite->setPosition( ccp(_mWinSize.width/2, _mWinSize.height/2));
	this->addChild(playerSprite, Z_ORDER_PLAYER);
	
	// Make player's wings
	playerSprite->addChild( SetWingAction() );

	b2BodyDef bodyDef2;
	bodyDef2.type = b2_dynamicBody;
	bodyDef2.position.Set(_mWinSize.width/2/PTM_RATIO, _mWinSize.height/2/PTM_RATIO);
	bodyDef2.userData = playerSprite;

	_mPlayerBody = _mWorld->CreateBody( &bodyDef2 );

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(playerSprite->getContentSize().width/PTM_RATIO/2, playerSprite->getContentSize().height/PTM_RATIO/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;

	_mPlayerBody->CreateFixture( &fixtureDef );
}

b2Body* MainGameScene::GetBodyAtTab(CCPoint p)
{
	b2Fixture* fix;
	for( b2Body* b = _mWorld->GetBodyList(); b; b = b->GetNext() )
	{
		if( b->GetUserData() != NULL )
		{
			if( b->GetType() == b2_staticBody ) continue;

			fix = b->GetFixtureList();
			if( fix->TestPoint( b2Vec2(p.x / PTM_RATIO, p.y/PTM_RATIO )) )
				return b;
		}
	}

	return nullptr;
}

void MainGameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();
		
		//Check if there is a cloud where user touches.
		if( _mCloudManager.DeleteCloudAt(touchPoint) )
			return;

		if( b2Body* b = this->GetBodyAtTab(touchPoint) )
		{
			if( !b )
				return;

			_mDragBody = b;
			_mOnDrag = true;

			b2MouseJointDef md;
			md.bodyA = _mJointBody;
			md.bodyB = _mDragBody;
			md.target.Set( _mDragBody->GetPosition().x, _mDragBody->GetPosition().y );
			md.maxForce = 300.0 * _mDragBody->GetMass();

			_mMouseJoint = (b2MouseJoint*)_mWorld->CreateJoint(&md);

			_mDragTouch = touchPoint;

			break;
		}
	}
}

void MainGameScene::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it )
	{
		CCTouch* touch = (CCTouch*)(*it);
		CCPoint touchPoint = touch->getLocation();

		if( _mOnDrag )
		{
			if( touchPoint.y > _mWinSize.height * FLYING_LINE )
			{
				_mWorld->DestroyJoint( _mMouseJoint );
				_mMouseJoint = NULL;

				_mDragBody->SetAwake( true );
				_mOnDrag = false;
				break;
			}
			_mMouseJoint->SetTarget( b2Vec2( touchPoint.x/PTM_RATIO, touchPoint.y/PTM_RATIO ));

			_mDragTouch = touchPoint;
		}
	}
}

void MainGameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	for( CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
	{
		if( _mOnDrag )
		{
			_mWorld->DestroyJoint( _mMouseJoint );
			_mMouseJoint = NULL;

			_mDragBody->SetAwake( true );

			SimpleAudioEngine::sharedEngine()->playEffect(SOUND_PRE_LOAD_FLY);
		}

		_mOnDrag = false;
	}
}

void MainGameScene::Reset()
{
	_mCloudManager.Reset();

	CCSprite* sprite = static_cast<CCSprite*>(_mPlayerBody->GetUserData());
	sprite->setPosition( ccp(_mWinSize.width/2, _mWinSize.height/2));

	b2Vec2 p;
	p.x = _mWinSize.width/2/PTM_RATIO;
	p.y = _mWinSize.height/2/PTM_RATIO;
	_mPlayerBody->SetTransform( p, 0.0);

	_mRecordManager.SetCurrentHeight(0);

	this->resumeSchedulerAndActions();

	_mSpriteHowToPlay->setPosition(ccp(0.0f, 0.0f));
	_mSpriteHowToPlay->setVisible(true);

	_mPlayerDie = false;

	_mOldDiff = 0;
}

CCSprite* MainGameScene::SetWingAction()
{
	CCSprite* wingSprite = CCSprite::createWithTexture(
		_mPlayerBatchNode->getTexture(),
		CCRectMake(0, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE)
		);
	wingSprite->setAnchorPoint(ccp(0, 0));

	CCAnimation* wingAnimation = CCAnimation::create();
	wingAnimation->setDelayPerUnit(0.05f);

	for(int i=0; i<6; ++i)
	{
		wingAnimation->addSpriteFrameWithTexture(
			_mPlayerBatchNode->getTexture(),
			CCRectMake(i*DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE)
			);
	}
	CCAnimate* wingAnimate = CCAnimate::create(wingAnimation);

	wingSprite->runAction( CCRepeatForever::create(wingAnimate));
	return wingSprite;
}

void MainGameScene::MoveHowToPlaySpriteDown(float length)
{
	if( !_mSpriteHowToPlay->isVisible() )	
		return;

	CCPoint curPoint = _mSpriteHowToPlay->getPosition();
	curPoint.y += length;
	
	// Delete the cloud which is moved under the bottom
	if( curPoint.y < _mSpriteHowToPlay->getContentSize().height * -1 )
	{
		_mSpriteHowToPlay->setVisible(false);
	}
	else
	{
		_mSpriteHowToPlay->setPosition(curPoint);	

		// Mode sprite that display the current mode has to be moved to down.
		_mMenuLayer->MoveModeSpriteDown(length);
	}
}

void MainGameScene::_SetPlayerDie()
{		
	_mMenuLayer->Awake();
	//_mMenuLayer->setVisible(true);

	_mRecordManager.SaveBestRecord();
	this->pauseSchedulerAndActions();

	_mPlayerBody->SetLinearVelocity(b2Vec2(0.0f, 20.0));

	// Display the current best record
	_mRecordManager.DisplayCurrentBestRecord();

	_mPlayerDie = true;
}






