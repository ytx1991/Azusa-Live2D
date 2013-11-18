/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */
#include "LAppLive2DManager.h"

#include "L2DViewMatrix.h"

//Live2DApplication
#include "LAppModel.h"
#include "LAppDefine.h"
#include "LAppModel.h"
#include "L2DMotionManager.h"


using namespace live2d;

/**
 * コンストラクタ
 */
LAppLive2DManager::LAppLive2DManager()
	:modelIndex(0)
{
	//  以下の命令でメモリリークの検査を実施（_DEBUGモードのみ）
	//　Live2D::dispose()時に、Live2Dの管理するメモリでリークが発生していた場合にダンプする
	//	リークしている場合は、MEMORY_DEBUG_MEMORY_INFO_ALLでより詳細な情報をダンプします
	//　検査用のデータはglobal new演算子を使います 
//	live2d::UtDebug::addMemoryDebugFlags( live2d::UtDebug::MEMORY_DEBUG_MEMORY_INFO_COUNT ) ;//メモリリークの検出用

	// Live2D初期化
	live2d::Live2D::init( &myAllocator );
}


/**
 * デストラクタ
 */
LAppLive2DManager::~LAppLive2DManager() 
{
	releaseModel();
	Live2D::dispose();
}


void LAppLive2DManager::releaseModel()
{
	for (unsigned int i=0; i<models.size(); i++)
	{
		delete models[i];
	}
    models.clear();
}


void LAppLive2DManager::setDrag(float x, float y)
{
	for (unsigned int i=0; i<models.size(); i++)
	{
		models[i]->setDrag(x, y);
	}
}


/**
 * タップしたときのイベント
 * @param tapCount
 * @return
 */
bool LAppLive2DManager::tapEvent(float x,float y)
{
	if(LAppDefine::DEBUG_LOG) UtDebug::print( "tapEvent\n");
	
	for (unsigned int i=0; i<models.size(); i++)
	{
		if(models[i]->hitTest(  HIT_AREA_HEAD,x, y ))
		{
			//顔をタップしたら表情切り替え
			if(LAppDefine::DEBUG_LOG)UtDebug::print( "face\n");
			models[i]->setRandomExpression();
		}
		else if(models[i]->hitTest( HIT_AREA_BODY,x, y))
		{
			if(LAppDefine::DEBUG_LOG)UtDebug::print( "body\n");
			models[i]->startRandomMotion(MOTION_GROUP_TAP_BODY, PRIORITY_NORMAL );
		}
	}
	    
    return true;
}


//モデルを追加する
void LAppLive2DManager::changeModel()
{
	if(LAppDefine::DEBUG_LOG)UtDebug::print("model index : %d\n",modelIndex );	
	switch (modelIndex)
	{
	case 0://ハル
		releaseModel();
		models.push_back(new LAppModel());
		models[0]->load( MODEL_HARU ) ;
		break;
	case 1://しずく
		releaseModel();
		models.push_back(new LAppModel());
		models[0]->load( MODEL_SHIZUKU ) ;
		break;
	case 2://わんこ
		releaseModel();
		models.push_back(new LAppModel());
		models[0]->load( MODEL_WANKO ) ;
		break;
	case 3://複数モデル
		releaseModel();
		models.push_back(new LAppModel());
		models[0]->load( MODEL_HARU_A ) ;
				
		models.push_back(new LAppModel());
		models[1]->load( MODEL_HARU_B ) ;
		break;
	default:		
		break;
	}
	modelIndex++;
	modelIndex = modelIndex%4;
}

void LAppLive2DManager::deviceLost(){
	if(LAppDefine::DEBUG_LOG) live2d::UtDebug::print( "DeviceLost @LAppLive2DManager::deviceLost()\n");

	for (unsigned int i=0; i<models.size(); i++)
	{
		models[i]->deviceLost() ;
	}
}
