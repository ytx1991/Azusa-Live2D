/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */
#include "LAppRenderer.h"

#include "LAppLive2DManager.h"
#include "LAppModel.h"
#include "LAppDefine.h"





LAppRenderer::LAppRenderer()
{

	//表示範囲の設定
	viewMatrix.identity() ;
	viewMatrix.setMaxScale( VIEW_MAX_SCALE );//限界拡大率
	viewMatrix.setMinScale( VIEW_MIN_SCALE );//限界縮小率

	//表示できる最大範囲
	viewMatrix.setMaxScreenRect(
			VIEW_LOGICAL_MAX_LEFT,
			VIEW_LOGICAL_MAX_RIGHT,
			VIEW_LOGICAL_MAX_BOTTOM,
			VIEW_LOGICAL_MAX_TOP
			);

	//仮初期化
	setDeviceSize( 800 , 600 ) ;//本来外部から設定するので不要だが設定を忘れても仮のサイズで描画されるように初期化
}


LAppRenderer::~LAppRenderer()
{
	
}

/************************************************************
	Live2D描画の標準的な座標系は画面中心を 0,0
	左上を(-1,1) , 右下を(1,-1)とする座標として設定する
************************************************************/
void LAppRenderer::setDeviceSize( int width , int height )
{
	live2d::UtDebug::println(" set Device size : %d , %d" , width , height ) ;

	//表示範囲
	float ratio=(float)height/width;
	float left = VIEW_LOGICAL_LEFT;
	float right = VIEW_LOGICAL_RIGHT;
	float bottom = -ratio;
	float top = ratio;
	viewMatrix.setScreenRect(left,right,bottom,top);//デバイスに対応する画面の範囲。 Xの左端, Xの右端, Yの下端, Yの上端

	float screenW=abs(left-right);

	deviceToScreen.identity() ;
	deviceToScreen.multTranslate(-width/2.0f,-height/2.0f );
	deviceToScreen.multScale( screenW/width , -screenW/width );
}



void LAppRenderer::draw()
{
	dragMgr.update();//ドラッグ用パラメータの更新

	live2DMgr->setDrag(dragMgr.getX(), dragMgr.getY());

	int numModels=live2DMgr->getModelNum();
	for (int i=0; i<numModels; i++)
	{
		LAppModel* model = live2DMgr->getModel(i);
		model->update();
		model->draw();
	}
}

void LAppRenderer::setLive2DManager(LAppLive2DManager* mgr)
{
	this->live2DMgr = mgr;
}


void LAppRenderer::scaleView(float cx,float cy,float scale)
{
	viewMatrix.adjustScale(cx,cy,scale);
}


void LAppRenderer::translateView(float shiftX,float shiftY)
{
	viewMatrix.adjustTranslate(shiftX,shiftY);
}


//マウスプレス時
void LAppRenderer::mousePress(int x,int y)
{
	float vx=transformDeviceToViewX( (float)x );
	float vy=transformDeviceToViewY( (float)y );

	if(LAppDefine::DEBUG_TOUCH_LOG) live2d::UtDebug::println( "mouse press / device(%4d,%4d) > logical( %5.3f , %5.3f )  @LAppRenderer#touchMove()" , x , y , vx , vy ) ;
	this->live2DMgr->tapEvent( vx , vy ) ;
}

//ドラッグ時
void LAppRenderer::mouseDrag(int x,int y)
{
	float vx=transformDeviceToViewX( (float)x );
	float vy=transformDeviceToViewY( (float)y );

	if(LAppDefine::DEBUG_TOUCH_LOG) live2d::UtDebug::println( "mouse drag / device(%4d,%4d) > logical( %5.3f , %5.3f )  @LAppRenderer#touchMove()" , x , y , vx , vy ) ;
	dragMgr.set(vx,vy);
}



void LAppRenderer::updateViewMatrix( float dx , float dy , float cx , float cy , float scale )
{
	bool isMaxScale=viewMatrix.isMaxScale();
	bool isMinScale=viewMatrix.isMinScale();
	
	//拡大縮小
	viewMatrix.adjustScale(cx, cy, scale);

	//移動(ホイールの場合は移動を伴わないので0,0(何もしない)。タッチの場合は移動＋拡大になる）
	viewMatrix.adjustTranslate(dx, dy) ;
	
	//画面が最大になったときのイベント
	if( ! isMaxScale)
	{
		if(viewMatrix.isMaxScale())
		{
			//最大表示になった時に何らかのイベント（アクション等）を行う場合はここに記述
			if(LAppDefine::DEBUG_LOG) live2d::UtDebug::println( "max scale" ) ;
		}
	}
	//画面が最小になったときのイベント
	if( ! isMinScale)
	{
		if(viewMatrix.isMinScale())
		{
			//最小表示になった時に何らかのイベント（アクション等）を行う場合はここに記述
			if(LAppDefine::DEBUG_LOG) live2d::UtDebug::println( "min scale" ) ;
		}
	}
	
}

//マウスホイール時
void LAppRenderer::mouseWheel( int delta , int x , int y ){
	float x_onScreen = deviceToScreen.transformX((float)x) ;
	float y_onScreen = deviceToScreen.transformY((float)y) ;

	//float scale = delta < 0 ? 0.5f : 2.0f ; 
	float scale = delta < 0 ? 1.0f/1.4142f : 1.41421f ; 
	

	//画面の拡大縮小、移動の設定
	updateViewMatrix( 0 , 0 , x_onScreen , y_onScreen , scale ) ;
}



float LAppRenderer::transformDeviceToViewX(float deviceX)
{
	float screenX = deviceToScreen.transformX( deviceX );	//論理座標変換した座標を取得。
	return  viewMatrix.invertTransformX(screenX);			//拡大、縮小、移動後の値。
}


float LAppRenderer::transformDeviceToViewY(float deviceY)
{
	float screenY = deviceToScreen.transformY( deviceY );	//論理座標変換した座標を取得。
	return  viewMatrix.invertTransformY(screenY);			//拡大、縮小、移動後の値。
}

