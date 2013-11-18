/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */

// Live2D
#include <stdlib.h>

#include	"Live2D.h"
#include	"util/UtDebug.h"



/************************************************************
* Live2D用メモリ確保命令のカスタマイズ
* live2d::LDAllocatorの仮想関数をオーバーライドする
************************************************************/
class MyLive2DAllocator : public live2d::LDAllocator{
public:

	
	MyLive2DAllocator() {
	}

	virtual ~MyLive2DAllocator() {
	}

	//---------------------------------------------------------------------------
	//	初期化処理
	//
	//  Live2D::init()から呼ばれる初期化処理。必要に応じて初期化処理を行う
	//---------------------------------------------------------------------------
	virtual void init(){ }

	//---------------------------------------------------------------------------
	//　終了処理
	//
	//　Live2D::dispose()の最後に呼ばれる終了処理。必要に応じて終了処理を行う
	//---------------------------------------------------------------------------
	virtual void dispose(){ }


	//---------------------------------------------------------------------------
	// malloc
	//
	// ※初期化時を除いてある程度のサイズ（1024以上）で呼ばれる
	//---------------------------------------------------------------------------
	virtual void* pageAlloc( unsigned int size , LDAllocator::Type  allocType ){
		void* ptr ;
		switch( allocType ){
		case LDAllocator::MAIN:	//通常のメモリ
			ptr = ::malloc(size) ;
			break ;

		case LDAllocator::GPU:	//GPUに渡す頂点、インデックス、UV
			ptr = ::malloc(size) ;
			break ;

		default:				//その他（今後追加する可能性あり）
			L2D_DEBUG_MESSAGE( "Alloc type not implemented %d" , allocType ) ;
			ptr = ::malloc(size) ;
			break ;
		}

		L2D_ASSERT_S( ptr != NULL , "MyAllocator#malloc failed (size= %d)" , size ) ;
		return ptr ;
	}

	//---------------------------------------------------------------------------
	// free
	//---------------------------------------------------------------------------
	virtual void pageFree( void* ptr , LDAllocator::Type  allocType ){
		::free(ptr);
	}

} ;

