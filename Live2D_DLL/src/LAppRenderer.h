/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */
#pragma once

#include "L2DViewMatrix.h"
#include "L2DTargetPoint.h"

class LAppLive2DManager;



class LAppRenderer
{
private:
	LAppLive2DManager*			live2DMgr;

	L2DViewMatrix				viewMatrix;
	L2DMatrix44 				deviceToScreen;
	L2DTargetPoint 				dragMgr;//ドラッグによるアニメーションの管理
public:
	LAppRenderer();
	~LAppRenderer();
	void setLive2DManager(LAppLive2DManager* mgr);
	void draw();

	void scaleView(float cx,float cy,float scale);
    void translateView(float x,float y); 

	void mousePress( int x , int y ) ;
	void mouseDrag(int x,int y);

	void mouseWheel( int delta , int x , int y ) ;
	void updateViewMatrix( float dx , float dy , float cx , float cy , float scale ) ;

	float transformDeviceToViewX(float deviceX);
	float transformDeviceToViewY(float deviceY);

	void setDeviceSize( int width , int height ) ;

	L2DViewMatrix* getViewMatrix(){ return &viewMatrix ; } 
};




