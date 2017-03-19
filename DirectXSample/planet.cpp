// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// planet.cpp v1.0

#include "planet.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Planet::Planet() : Entity()
{
	spriteData.x = planetNS::X;              // 画面上の位置
	spriteData.y = planetNS::Y;
	radius = planetNS::COLLISION_RADIUS;
	mass = planetNS::MASS;
	startFrame = planetNS::START_FRAME;		// 惑星アニメーションの最初のフレーム
	endFrame = planetNS::END_FRAME;			// 惑星アニメーションの最初のフレーム
	setCurrentFrame(startFrame);
}