
#ifndef __CLOUD_MANAGER_H__
#define __CLOUD_MANAGER_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

/** 
 @brief Managing the clouds 
 */
class CCloudManager
{
public:
	CCloudManager();
	~CCloudManager();

	/** Make the new clouds based on current meter
	The higher character fly, The many clouds can be made
	@param curMeter current mether.
	*/
	void MakeCloud(CCLayer* layer, int curMeter);

	/** Check if there is a cloud at touch point.
	If so, return true
	If not, return false 
	*/
	bool DeleteCloudAt(CCPoint point);

	/** Move the all clouds to down */
	void MoveDownAllClouds(CCLayer* layer, float length);

	void AnimateTouchedCloud(int index);

	/** Remove all clouds and clear the vector */
	void Reset(CCLayer* layer);

private:
	CCTexture2D*			_mCloudTexture;
	std::vector<CCSprite*>	_mCloudSprites;

	CCSize					_mWinSize;
};

#endif