/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*			®q°–®r©©~ °Ë°° ®q°–®r ®q°–®r 
*			®q°–®q°–®r®q°–®r°´®q°–®r¶·¶ÅE°°¶·¶ÅE
*			,¶·¶·¶·¶ÅE''¶·~~ ,''~¶·¶·°° ,'' 
*			
*			
*				  ("`-''-/").___..--''"`-._
*				   `6_ 6  )   `-.  (     ).`-.__.`)
*				   (_Y_.)'  ._   )  `._ `. ``-..-'
*				 _..`--'_..-_/  /--'_.' ,'
*				(il),-'   (li),'  ((!.-'  
*
* 	SHINE Himself Is Not Enigma ... 
* 	RYU's Yokel Union
\ * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name:	uImageDC.h
* Date:			06/09/2007
* Author:		Shine Ryu
* Email:		ShineRyu@126.com
* Description:		
* 	
* -------------------------------------------------------------------------
* Copyright (c) 2007, Shine Ryu <ShineRyu@126.com>, P.R.China.
* All rights reserved.
*
* LICENSE TERMS
*
* The free distribution and use of this software in both source and binary 
* form is allowed (with or without changes) provided that:
*
*   1. distributions of this source code include the above copyright 
*      notice, this list of conditions and the following disclaimer;
*
*   2. distributions in binary form include the above copyright
*      notice, this list of conditions and the following disclaimer
*      in the documentation and/or other associated materials;
*
*   3. the copyright holder's name is not used to endorse products 
*      built using this software without specific written permission. 
*
* DISCLAIMER
*
* This software is provided 'as is' with no explcit or implied warranties
* in respect of any properties, including, but not limited to, correctness 
* and fitness for purpose.
* -------------------------------------------------------------------------
* Issue Date: 06/09/2007
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippiImageDC.h: interface for the CUImageDC class.
//            It provides device context with bitmap to draw the image
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_IMAGEDC_H__A36429AA_4E72_4A0C_8A39_C114CD0CFF5E__INCLUDED_)
//#define AFX_IMAGEDC_H__A36429AA_4E72_4A0C_8A39_C114CD0CFF5E__INCLUDED_
//
//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000

class CUImageDC
{
public:

    CUImageDC();
    virtual ~CUImageDC();

	void Delete();
	void Create(int nWidth, int nHeight);

	HDC GetSafeHdc(void){
		return m_hDC;
	}
	HBITMAP GetBmpHandle(void){
		return m_hBmp;
	}
	DWORD* GetBits(void){
		return (DWORD*)m_pBits;
	}

protected:

	HBITMAP	m_hBmp;
	HDC		m_hDC;
	char*   m_pBits;
};

//#endif // !defined(AFX_IMAGEDC_H__A36429AA_4E72_4A0C_8A39_C114CD0CFF5E__INCLUDED_)
