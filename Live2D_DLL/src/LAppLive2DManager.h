/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */
#pragma once

#include "type/LDVector.h"
#include <math.h>
#include "MyLive2DAllocator.h"


class LAppModel;
class L2DViewMatrix;

class LAppLive2DManager{
private :
	//モデルデータ	
	// モデルの番号
public:
    	int modelIndex;
		live2d::LDVector<LAppModel*> models;
			MyLive2DAllocator	myAllocator ;
    LAppLive2DManager() ;    
    ~LAppLive2DManager() ; 
    
	void init();
	void releaseModel();
    LAppModel* getModel(int no){ return models[no]; }
	
    int getModelNum(){return models.size();}
    bool tapEvent(float x,float y) ;
    void setDrag(float x, float y);
	void changeModel();

	void deviceLost() ;

};

