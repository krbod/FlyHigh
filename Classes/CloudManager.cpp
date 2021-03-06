#include "CloudManager.h"
#include "RandGenerator.h"
#include "AppMacros.h"

const int LEVEL_UPGRADED_PER_METER = 50;

CCloudManager::CCloudManager()
{
	_mCloudTexture = CCTextureCache::sharedTextureCache()->addImage(IMAGE_CLOUD_PATH);
	_mWinSize = CCDirector::sharedDirector()->getWinSize();
}


CCloudManager::~CCloudManager()
{
}


void CCloudManager::MakeCloud(CCLayer* layer, int curMeter)
{
	int level;

	// level up each 50m.
	if( curMeter > LEVEL_UPGRADED_PER_METER )
		level = curMeter/LEVEL_UPGRADED_PER_METER + 1;
	else 
		level = 1;

	int cloudCount = CRandGenerator::GetAnyInt(0, level);

	for(int i=0; i<cloudCount; ++i)
	{
		CCSprite* cloudSprite = CCSprite::createWithTexture(_mCloudTexture);	
		cloudSprite->setPosition( ccp(CRandGenerator::GetAnyFloat(0, _mWinSize.width), _mWinSize.height));
		layer->addChild(cloudSprite, Z_ORDER_CLOUD );

		_mCloudSprites.push_back(cloudSprite);
	}

}

bool CCloudManager::DeleteCloudAt(CCPoint point)
{
	for(unsigned int i=0; i<_mCloudSprites.size(); ++i)
	{
		if( _mCloudSprites[i]->boundingBox().containsPoint( point ) )
		{
			// Animate the cloud on touch point 
			AnimateTouchedCloud(i);
			return true;
		}
	}

	return false;
}

void CCloudManager::MoveDownAllClouds(CCLayer* layer, float length)
{
	for(unsigned int i=0; i<_mCloudSprites.size(); ++i)
	{
		CCPoint curPoint = _mCloudSprites[i]->getPosition();
		curPoint.y += length;

		// Delete the cloud which is moved under the bottom
		if( curPoint.y < _mCloudTexture->getContentSize().height * -1 )
		{
			layer->removeChild( _mCloudSprites[i] );
			_mCloudSprites.erase(_mCloudSprites.begin() + i);
			--i;
		}
		else
		{
			_mCloudSprites[i]->setPosition(curPoint);			
		}
	}
}

void CCloudManager::Reset(CCLayer* layer)
{
	for(unsigned int i=0; i<_mCloudSprites.size(); ++i)
	{
		layer->removeChild(_mCloudSprites[i]);
	}

	_mCloudSprites.clear();
}

void CCloudManager::AnimateTouchedCloud(int index)
{
	CCPoint origin = _mCloudSprites[index]->getPosition();

	CCActionInterval* actionRight = CCMoveTo::create(0.1f, ccp(origin.x + 10, origin.y) );
	CCActionInterval* actionLeft = CCMoveTo::create(0.1f, ccp(origin.x - 10, origin.y ));

	CCFiniteTimeAction* action = CCSequence::create(actionRight, actionLeft, NULL);

	CCActionInterval* repeat = CCRepeat::create(action, 5);

	_mCloudSprites[index]->runAction(repeat);
}
