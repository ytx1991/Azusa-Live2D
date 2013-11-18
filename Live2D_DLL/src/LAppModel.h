/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */
#pragma once

#include "L2DBaseModel.h"
#include "Live2DModelD3D.h"
#include "type/LDVector.h"

#include "ModelSetting.h"
#include "L2DViewMatrix.h"

class LAppModel : public L2DBaseModel
{
private:

			

	live2d::LDVector<LPDIRECT3DTEXTURE9>	textures ;	//モデルのテクスチャ

public:
    LAppModel();
    ~LAppModel(void);
        ModelSetting*				modelSetting;//モデルの設定
	live2d::LDString			modelHomeDir;
	 char* ModelPath;
	 float faceX,faceY,faceZ,bodyX,eyeX,eyeY;
    live2d::Live2DModelD3D*		live2DModel;
	void load(int modelIndex);
	void load(const char* path) ;
	
    void update();
    void draw();
	

    int startMotion(const char name[],int no,int priority);
	int startRandomMotion(const char name[],int priority);
	
	  void setExpression(const char name[]);
	void setRandomExpression();
	
	void loadModelData( const char fileName[],live2d::LDVector<live2d::LDString>& texFiles);
	void loadPhysics(const char fileName[]);
	void loadExpressions(live2d::LDVector<live2d::LDString>& names,live2d::LDVector<live2d::LDString>& files);
	void loadPose(const char fileName[]);
	void preloadMotionGroup(const char name[]);
    
	bool hitTest(const char pid[],float testX,float testY);
	live2d::ALive2DModel* getLive2DModel(){return live2DModel;}

	void deviceLost() ;
};






