/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */

#include <string>


//Live2D Application
#include "LAppModel.h"
#include "LAppDefine.h"

#include "FileManager.h"
#include "ModelSettingJson.h"
#include "util\UtSystem.h"

using namespace live2d;


//D3Dデバイス
extern LPDIRECT3DDEVICE9		g_pD3DDevice ;




LAppModel::LAppModel()
	:L2DBaseModel(),modelSetting(NULL),live2DModel(NULL)
{
	eyeX=0;eyeY=0;
	bodyX=0;
	faceX=0;faceY=0;faceZ=0;
	if (LAppDefine::DEBUG_LOG)
	{
		mainMotionMgr->setMotionDebugMode(true);
	}
}


LAppModel::~LAppModel(void)
{
	if(LAppDefine::DEBUG_LOG)UtDebug::print("delete model\n");
	delete modelSetting;
	delete live2DModel;

	for(unsigned int i=0;i<textures.size();i++)
	{
		textures[i]->Release() ;
	}
	textures.clear();
}


void LAppModel::load(int modelIndex)
{
	const char* path = NULL ;
	switch (modelIndex)
	{
	case MODEL_HARU:
		path = "res\\live2d\\haru\\haru.model.json" ;
		break;
	case MODEL_SHIZUKU:
		path = "res\\live2d\\shizuku\\shizuku.model.json" ;
		break;
	case MODEL_WANKO:
		path = "res\\live2d\\wanko\\wanko.model.json" ;
		break;
	case MODEL_HARU_A:
		path = "res\\live2d\\haru\\haru_01.model.json" ;
		break;
	case MODEL_HARU_B:
		path = "res\\live2d\\haru\\haru_02.model.json" ;
		break;
	default:		
		break;
	}

	if( path == NULL ){
		UtDebug::print( "Not supported model no : %d @ LAppModel::load()\n",modelIndex );	
		return ;
	}

	load(path) ;
}


void LAppModel::load(const char* path)
{
	ModelPath=new char[strlen(path)+1];
	strcpy(ModelPath,path);
	if(LAppDefine::DEBUG_LOG) UtDebug::print( "load model : %s\n",path);	
    updating=true;
    initialized=false;
    
	int size ;

	char* data = FileManager::loadFile( path , &size ) ;
    modelSetting = new ModelSettingJson( data , size );

	FileManager::releaseBuffer(data);
	
	//JSONの入っているフォルダをmodelHomeDirにセット
	FileManager::getParentDir( path , &modelHomeDir ) ;

    if(LAppDefine::DEBUG_LOG) UtDebug::print( "create model : %s\n",modelSetting->getModelName());	
    updating=true;
    initialized=false;

   //モデルのロード
    if( strcmp( modelSetting->getModelFile() , "" ) != 0 )
    {        
        LDString modelFile=modelSetting->getModelFile();
        
		LDVector<LDString> texFiles;
		int len=modelSetting->getTextureNum();
		for (int i=0; i<len; i++)
		{
			texFiles.push_back(modelSetting->getTextureFile(i));
		}
		loadModelData(modelFile.c_str(),texFiles);
    }
	
	if (live2DModel==NULL) {

		return;
	}

    //表情
	if (modelSetting->getExpressionNum() > 0)
	{
		LDVector<LDString> names;
		LDVector<LDString> files;
		int len=modelSetting->getExpressionNum();
		for (int i=0; i<len; i++)
		{
			names.push_back(modelSetting->getExpressionName(i));
			files.push_back(modelSetting->getExpressionFile(i));
		}
        loadExpressions(names,files);
	}
	
	//物理演算
	if( strcmp( modelSetting->getPhysicsFile(), "" ) != 0 )
    {        
        LDString path=modelSetting->getPhysicsFile();
        loadPhysics(path.c_str());
    }
	
	//ポーズ
	if( strcmp( modelSetting->getPoseFile() , "" ) != 0 )
    {
        LDString path=modelSetting->getPoseFile();
        loadPose(path.c_str());
    }
	//目パチ
	if (eyeBlink==NULL)
	{
		eyeBlink=new L2DEyeBlink();
	}
	
	//レイアウト
	LDMap<LDString, float> layout;
	if (modelSetting->getLayout(layout) )
	{
		modelSetting->getLayout(layout);
		LDMap<LDString, float>::const_iterator ite;
		
		for (ite=layout.begin(); ite!=layout.end(); ite++) {
			LDString key=(*ite).first;
			float value=(*ite).second;
			if (key=="width")modelMatrix->setWidth(value);
			else if (key=="height" )	modelMatrix->setHeight(value);
		}
		
		for (ite=layout.begin(); ite!=layout.end(); ite++) {
			LDString key=(*ite).first;
			float value=(*ite).second;
			if (key=="x" )modelMatrix->setX(value);
			else if (key=="y" )modelMatrix->setY(value);
			else if (key=="center_x" )modelMatrix->centerX(value);
			else if (key=="center_y" )modelMatrix->centerY(value);
			else if (key=="top")modelMatrix->top(value);
			else if (key=="bottom" )modelMatrix->bottom(value);
			else if (key=="left" )modelMatrix->left(value);
			else if (key=="right" )modelMatrix->right(value);
		}
	}
	
	for ( int i = 0; i < modelSetting->getInitParamNum(); i++)
	{
		live2DModel->setParamFloat(modelSetting->getInitParamID(i), modelSetting->getInitParamValue(i));
	}

	for ( int i = 0; i < modelSetting->getInitPartsVisibleNum(); i++)
	{
		live2DModel->setPartsOpacity(modelSetting->getInitPartsVisibleID(i), modelSetting->getInitPartsVisibleValue(i));
	}
	
	live2DModel->saveParam();

	preloadMotionGroup(MOTION_GROUP_IDLE);
	
	mainMotionMgr->stopAllMotions();
	
    updating=false;//更新状態の完了
    initialized=true;//初期化完了
}


void LAppModel::loadModelData( const char modelFile[],LDVector<LDString>& texFiles)
{
	//すでにデータが有る場合は削除
	if (live2DModel!=NULL)
	{
		delete live2DModel;	//moc
		delete modelMatrix;	//変換行列

		for(unsigned int i=0;i<textures.size();i++)
		{
			textures[i]->Release() ;
		}
		textures.clear();
	}

	//読み込み開始
	LDString modelPath = modelHomeDir + modelFile ;
	if(LAppDefine::DEBUG_LOG)UtDebug::print("modelPath %s\n", modelPath.c_str());
	live2DModel = Live2DModelD3D::loadModel(modelPath);
	live2DModel->setDevice( g_pD3DDevice ) ;
	
	if (Live2D::getError()!=Live2D::L2D_NO_ERROR) {
		UtDebug::print("failed load\n");
		return;
	}
	
	if(LAppDefine::DEBUG_LOG)UtDebug::print("load model w:%.0f,h:%.0f\n",live2DModel->getCanvasWidth(),live2DModel->getCanvasHeight());
	
	int len=texFiles.size();
	for (int i=0; i<len; i++)
	{
		LPDIRECT3DTEXTURE9	texture ;
		textures.push_back(texture);

		LDString texPath = modelHomeDir + texFiles[i];

		FileManager::loadTexture( g_pD3DDevice , texPath.c_str(),&textures[i]) ;

		live2DModel->setTexture( i , textures[i] ) ;//テクスチャとモデルを結びつける
	}
	
	//モデル変換行列の初期設定
	modelMatrix=new L2DModelMatrix(live2DModel->getCanvasWidth(),live2DModel->getCanvasHeight());
	modelMatrix->setWidth(2);
	modelMatrix->setCenterPosition(0, 0);
}


void LAppModel::loadPhysics(const char fileName[])
{
	//すでにデータが有る場合は削除
	if (physics!=NULL)
	{
		delete physics;
	}
	
	LDString path = modelHomeDir + fileName ;

	//読み込み開始
	int size;
	void* data=FileManager::loadFile(path.c_str() ,&size);
	physics= L2DPhysics::load(data,size) ;
	FileManager::releaseBuffer(data);
}


void LAppModel::loadExpressions(LDVector<LDString>& names,LDVector<LDString>& files)
{
	//すでにデータが有る場合は削除
	releaseExpressions();
	
	expressionMgr=new L2DMotionManager();
	//読み込み開始
	int len = names.size();
    for (int i = 0; i < len; i++)
	{
		LDString& name = names[i];

		LDString path = modelHomeDir + files[i] ;

		int size;
		void* data=FileManager::loadFile( path.c_str(),&size);
        
        L2DExpressionMotion* motion = L2DExpressionMotion::loadJson(data,size);
       
        expressions[name]= motion ;

		FileManager::releaseBuffer(data);
    }
}


void LAppModel::loadPose(const char fileName[])
{
	//すでにデータが有る場合は削除
	if (pose!=NULL)
	{
		delete pose;
	}

	LDString path = modelHomeDir + fileName ;

	//読み込み開始
	int size;
	void* data=FileManager::loadFile(path.c_str() ,&size);

	pose= L2DPose::load(data,size) ;

	FileManager::releaseBuffer(data);
}


void LAppModel::preloadMotionGroup(const char name[])
{
    int len = modelSetting->getMotionNum( name );
    for (int i = 0; i < len; i++)
	{
		LDString motionName= modelSetting->getMotionFile(name,i);
		LDString path = modelHomeDir + motionName ;

		if(LAppDefine::DEBUG_LOG)UtDebug::print("load motion name:%s \n",path.c_str() );
        
		AMotion* motion = Live2DMotion::loadMotion(path.c_str());
        motion->setFadeIn(  modelSetting->getMotionFadeIn(name,i)  );
        motion->setFadeOut( modelSetting->getMotionFadeOut(name,i) );
        motions[motionName]= motion ;
    }
}


void LAppModel::update()
{
	//-----------------------------------------------------------------
	live2DModel->loadParam();//前回セーブされた状態をロード
	if(mainMotionMgr->isFinished())
	{
		//モーションの再生がない場合、待機モーションの中からランダムで再生する
		startRandomMotion(MOTION_GROUP_IDLE, PRIORITY_IDLE);
	}
	else
	{
		bool update = mainMotionMgr->updateParam(live2DModel);//モーションを更新
		
		if( ! update){
			//メインモーションの更新がないとき
			eyeBlink->setParam(live2DModel);//目パチ
		}
	}
	live2DModel->saveParam();//状態を保存
	//-----------------------------------------------------------------
	
	
	if(expressionMgr!=NULL)expressionMgr->updateParam(live2DModel);// 表情でパラメータ更新（相対変化）
	
	
	//ドラッグによる変化
	//ドラッグによる顔の向きの調整
	live2DModel->addToParamFloat( PARAM_ANGLE_X, faceX *  30 , 1 );//-30から30の値を加える
	live2DModel->addToParamFloat( PARAM_ANGLE_Y, faceY *  30 , 1 );
	//live2DModel->addToParamFloat( PARAM_ANGLE_Z, (faceX*faceY) * -30 , 1 );
	live2DModel->addToParamFloat( PARAM_ANGLE_Z, faceZ* 30 , 1 );
	//ドラッグによる体の向きの調整
	live2DModel->addToParamFloat( PARAM_BODY_X    , bodyX * 10 , 1 );//-1から1の値を加える
	
	//ドラッグによる目の向きの調整
	live2DModel->addToParamFloat( PARAM_EYE_BALL_X, eyeX  , 1 );//-1から1の値を加える
	live2DModel->addToParamFloat( PARAM_EYE_BALL_Y, eyeY  , 1 );
	
	//呼吸など
	LDint64	 timeMSec = UtSystem::getUserTimeMSec() - startTimeMSec  ;
	double t = (timeMSec / 1000.0) * 2 * 3.14159  ;//2πt
	
	live2DModel->addToParamFloat( PARAM_ANGLE_X,	(float) (15 * sin( t/ 6.5345 )) , 0.5f);//-15 ~ +15 まで周期的に加算。周期は他とずらす。
	live2DModel->addToParamFloat( PARAM_ANGLE_Y,	(float) ( 8 * sin( t/ 3.5345 )) , 0.5f);
	live2DModel->addToParamFloat( PARAM_ANGLE_Z,	(float) (10 * sin( t/ 5.5345 )) , 0.5f);
	live2DModel->addToParamFloat( PARAM_BODY_X,	(float) ( 4 * sin( t/15.5345 )) , 0.5f);
	live2DModel->setParamFloat  ( PARAM_BREATH,	(float) (0.5f + 0.5f * sin( t/3.2345 )),1);//0~1 まで周期的に設定。モーションを上書き。
	
	
	if(physics!=NULL)physics->updateParam(live2DModel);//物理演算でパラメータ更新

	//リップシンクの設定
	if(lipSync)
	{
		float value = 0;//リアルタイムでリップシンクを行う場合、システムから音量を取得して0～1の範囲で入力してください。
		live2DModel->setParamFloat(PARAM_MOUTH_OPEN_Y, value ,0.8f);
	}
	
	//ポーズの設定
	if(pose!=NULL)pose->updateParam(live2DModel);

	live2DModel->update();
}


int LAppModel::startMotion(const char name[],int no,int priority)
{
	if (! mainMotionMgr->reserveMotion(priority))
	{
		if(LAppDefine::DEBUG_LOG)UtDebug::print("can't start motion.\n");
		return -1;
	}
	LDString motionFile = modelSetting->getMotionFile(name, no);
	AMotion* motion = motions[motionFile];
	bool autoDelete = false;
	if ( motion == NULL )
	{
		//読み込み
		LDString path = modelHomeDir + motionFile ;
		motion = Live2DMotion::loadMotion(path.c_str());
		
		motion->setFadeIn(  modelSetting->getMotionFadeIn(name,no)  );
		motion->setFadeOut( modelSetting->getMotionFadeOut(name,no) );
		
		autoDelete = true;//終了時にメモリから削除
	}
	
    if(LAppDefine::DEBUG_LOG)UtDebug::print("start motion ( %s : %d )\n",name,no);

	return mainMotionMgr->startMotionPrio(motion,autoDelete,priority);
}


int LAppModel::startRandomMotion(const char name[],int priority)
{
	if(modelSetting->getMotionNum(name)==0)return -1;
    int no = rand() % modelSetting->getMotionNum(name); 
    
    return startMotion(name,no,priority);
}


/**
 * モデルを描画する。
 * プラットフォームごとの固有設定も行う。
 * モデルが設定されてない場合は何もしない。
 * @param gl
 */
void LAppModel::draw()
{
    if (live2DModel == NULL)return;

	// 座標変換退避
	D3DXMATRIXA16 buf ;
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &buf);//World座標を取得

	// モデルの変換を適用
	float* tr = modelMatrix->getArray() ;//float[16]
	g_pD3DDevice->MultiplyTransform( D3DTS_WORLD , (D3DXMATRIXA16*)tr ) ;

	// Live2Dを描画
	live2DModel->draw();

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &buf);//変換を復元
}


/**
 * 当たり判定との簡易テスト。
 * 指定IDの頂点リストからそれらを含む最大の矩形を計算し、点がそこに含まれるか判定
 *
 * @param id
 * @param testX
 * @param testY
 * @return
 */
bool LAppModel::hitTest(const char pid[],float testX,float testY)
{
	if(alpha<1)return false;//透明時は当たり判定なし。
	int len=modelSetting->getHitAreasNum();
	for (int i = 0; i < len; i++)
	{
		if( strcmp( modelSetting->getHitAreaName(i) ,pid) == 0 )
		{
			const char* drawID=modelSetting->getHitAreaID(i);
			int drawIndex=live2DModel->getDrawDataIndex(drawID);
			if(drawIndex<0)return false;//存在しない場合はfalse
			int count=0;
			float* points=live2DModel->getTransformedPoints(drawIndex,&count);
			
			float left=live2DModel->getCanvasWidth();
			float right=0;
			float top=live2DModel->getCanvasHeight();
			float bottom=0;
			
			for (int j = 0; j < count; j++)
			{
				float x = points[DEF::VERTEX_OFFSET+j*DEF::VERTEX_STEP];
				float y = points[DEF::VERTEX_OFFSET+j*DEF::VERTEX_STEP+1];
				if(x<left)left=x;	// 最小のx
				if(x>right)right=x;	// 最大のx
				if(y<top)top=y;		// 最小のy
				if(y>bottom)bottom=y;// 最大のy
			}
			float tx=modelMatrix->invertTransformX(testX);
			float ty=modelMatrix->invertTransformY(testY);
			
			return ( left <= tx && tx <= right && top <= ty && ty <= bottom ) ;
		}
	}
	return false;//存在しない場合はfalse
}

//表情设置
  void LAppModel::setExpression(const char expressionID[])
{
	AMotion* motion = expressions[expressionID] ;
	if(LAppDefine::DEBUG_LOG)UtDebug::print( "expression[%s]\n" , expressionID ) ;
	if( motion != NULL )
	{
		expressionMgr->startMotion(motion, false) ;
	}
	else
	{
		if(LAppDefine::DEBUG_LOG)UtDebug::print( "expression[%s] is null \n" , expressionID ) ;
	}
}


void LAppModel::setRandomExpression()
{
	int no=rand()%expressions.size();
	LDMap<LDString,AMotion* >::const_iterator map_ite;
	int i=0;
	for(map_ite=expressions.begin();map_ite!=expressions.end();map_ite++)
	{
		if (i==no)
		{
			LDString name=(*map_ite).first;
			setExpression(name.c_str());
			return;
		}
		i++;
	}
}


void LAppModel::deviceLost() {
	live2DModel->deviceLostD3D() ;
}
