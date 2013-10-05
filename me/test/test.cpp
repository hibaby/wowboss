// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WINDOWS.H>
#include <time.h>
#include "../FxjFunc.h"
#include <fstream>

typedef struct tagCALCINFOx
{
	DWORD		m_dwSize;				//结构大小
	DWORD		m_dwVersion;			//调用软件版本(V2.10 : 0x210)
	DWORD		m_dwSerial;				//调用软件序列号
	char*		m_strStkLabel;			
	BOOL		m_bIndex;				
	
	int			m_nNumData;				//数据数量(pData,pDataEx,pResultBuf数据数量)
	STKDATA*	m_pData;				//常规数据,注意:当m_nNumData==0时可能为 NULL
	STKDATAEx*	m_pDataEx;				//扩展数据,,注意:可能为 NULL
	
	int			m_nParam1Start;			//参数1有效位置
	float*		m_pfParam1;				//调用参数1	
	float*		m_pfParam2;				//调用参数2
	float*		m_pfParam3;				//调用参数3
	float*		m_pfParam4;				//调用参数3
	
	float*		m_pResultBuf;			//结果缓冲区
	DATA_TYPE	m_dataType;				//数据类型
	float*		m_pfFinData;			
} CALCINFOX;

const int NUMDATA = 10;
STKDATA		g_STKData[NUMDATA] = {
	{ 0, 10.01f, 20.02f, 0.03f, 20.05f, 100.06f, 0.07f, 8, 9 }
	,{ 1, 11.01f, 21.02f, 1.03f, 21.05f, 101.06f, 1.07f, 8, 9 }
	,{ 2, 12.01f, 22.02f, 2.03f, 22.05f, 102.06f, 2.07f, 8, 9 }
};
STKDATAEx	g_STKDataEx[NUMDATA];

float	g_fParam1[3] = {1.0f, 2.0f, 3.0f};
float	g_fParam2 = 1.0f;
float	g_fParam3 = 3.0f;
float	g_fParam4 = 4.0f;

float		g_ResultBuf[NUMDATA];

void MakeData()
{
	FILE* f = fopen("me.txt", "r");
	if ( f )
	{
		char szLine[256];
		fgets(szLine, sizeof(szLine)-1, f);
		fgets(szLine, sizeof(szLine)-1, f);
		fgets(szLine, sizeof(szLine)-1, f);

		int index = 0;
		while( fgets(szLine, sizeof(szLine)-1, f) )
		{
			char szData[256];
			char szTime[256];
			int amount;
			int amount2;
			float ma[6];

			sscanf( szLine, "%s %s"
				"\t%f\t%f\t%f\t%f\t%f\t%d"
				"\t%d"
				"\t%f\t%f\t%f\t%f\t%f\t%f\n"
				, szData, szTime
				,&g_STKData[index].m_fOpen
				,&g_STKData[index].m_fHigh
				,&g_STKData[index].m_fLow
				,&g_STKData[index].m_fClose
				,&g_STKData[index].m_fVolume
				,&amount
				,&amount2
				,&ma[0],&ma[1],&ma[2],&ma[3],&ma[4],&ma[5],&ma[6]);
			g_STKData[index].m_fAmount = amount*1.0f;

			++index;
			if ( index >= NUMDATA )
			{
				break;
			}
		}
		fclose(f);
	}
}

typedef int (CALLBACK *MyFunc)(CALCINFOX* pData);
int main(int argc, char* argv[])
{
	MakeData();
	CALCINFOX	g_CalcInfox;
	g_CalcInfox.m_strStkLabel	= "3333";
	g_CalcInfox.m_dataType		= MIN1_DATA;
	g_CalcInfox.m_nNumData		= NUMDATA;
	g_CalcInfox.m_pData			= g_STKData;
	g_CalcInfox.m_pDataEx		= g_STKDataEx;

	g_CalcInfox.m_nParam1Start	= 0;
	g_CalcInfox.m_pfParam1		= g_fParam1;
	g_CalcInfox.m_pfParam2		= &g_fParam2;
	g_CalcInfox.m_pfParam3		= &g_fParam3;
	g_CalcInfox.m_pfParam4		= &g_fParam4;

	g_CalcInfox.m_pResultBuf	= g_ResultBuf;

	HMODULE hm = LoadLibrary("me.dll");
	if ( hm )
	{
		MyFunc fpFun = (MyFunc)GetProcAddress(hm,"_CALLFUNC@4");
		(*fpFun)(&g_CalcInfox);
		MyFunc fpFun2 = (MyFunc)GetProcAddress(hm,"_CALLFUNCVAR@4");
		(*fpFun2)(&g_CalcInfox);

		FreeLibrary(hm);
	}
	return 0;
}
