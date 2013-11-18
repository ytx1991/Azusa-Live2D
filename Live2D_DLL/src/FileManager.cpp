/**
 *
 *  このソースはLive2D関連アプリの開発用途に限り
 *  自由に改変してご利用頂けます。
 *
 *  (c) CYBERNOIDS Co.,Ltd. All rights reserved.
 */

#include "FileManager.h"
#include "MyLive2DAllocator.h"
#include <stdio.h>
#include "type/LDString.h"


char* FileManager::loadFile(const char* filepath,int* ret_bufsize)
{
		FILE *fp; // (1)ファイルポインタの宣言
		char * buf;
	
		// (2)ファイルのオープン。ここで、ファイルポインタを取得する
		if ( fopen_s( &fp , filepath, "rb") ) //return nonzero if error
		{
			L2D_DEBUG_MESSAGE("file open error %s!!" , filepath );
			return NULL;
		}
	
		//------------ サイズの判定 ------------
		fseek(fp, 0, SEEK_END );
		int size = ftell(fp);//サイズを取得
	
		buf = (char*)malloc( size );//メモリの確保（外部で開放）
		L2D_ASSERT_S( buf != 0 , "malloc( %d ) is NULL @ fileload %s" , size , filepath ) ;
		
	
		fseek(fp, 0, SEEK_SET);
	
		// 読み込み
		int loaded = (int)fread(buf, sizeof(char), size, fp);
		fclose(fp); // (5)ファイルのクローズ
	
		//------------ ただしく読み込めたか判定 ------------
		if (loaded != size)
		{
			L2D_DEBUG_MESSAGE("file load error / loaded size is wrong / %d != %d\n" , loaded , size );
	
	
			return NULL;
		}
	
		*ret_bufsize = size ;
		return buf;

}


void FileManager::releaseBuffer(void* ptr)
{
	free(ptr);
}


/************************************************************
@title	テクスチャ読み込み
@subt
************************************************************/
void FileManager::loadTexture( LPDIRECT3DDEVICE9 g_pD3DDevice, const char * textureFilePath, LPDIRECT3DTEXTURE9* tex)
{
	if( FAILED( D3DXCreateTextureFromFileExA( g_pD3DDevice
				, textureFilePath 
				, 0	//width 
				, 0	//height
				, 0	//mipmap( 0なら完全なミップマップチェーン）
				, 0	//Usage
				, D3DFMT_A8R8G8B8                
				, D3DPOOL_MANAGED
				, D3DX_FILTER_LINEAR
				, D3DX_FILTER_BOX
				, 0			  
				, NULL
				, NULL
				, tex ) ) )
	{
		L2D_DEBUG_MESSAGE("Could not create texture \n" , textureFilePath );
		return ;
	}
}


/************************************************************
@title	親ディレクトリの取得
@subt
************************************************************/
void FileManager::getParentDir( const char* path , live2d::LDString* return_dir ){
	(*return_dir) = path ;
	(*return_dir) += "\\..\\" ;

}
