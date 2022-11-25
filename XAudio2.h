//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// サンプルBGM
	SOUND_LABEL_BGM001,			// サンプルBGM
	SOUND_LABEL_BGM002,			// サンプルBGM
	SOUND_LABEL_BGM003,			// サンプルBGM
	SOUND_LABEL_BGM004,			// サンプルBGM
	SOUND_LABEL_BGM005,			// サンプルBGM
	SOUND_LABEL_BGM006,			// サンプルBGM
	SOUND_LABEL_SE000,			// サンプルSE
	SOUND_LABEL_SE001,			// サンプルSE
	SOUND_LABEL_SE002,			// サンプルSE
	SOUND_LABEL_SE003,			// サンプルSE
	SOUND_LABEL_SE004,			// サンプルSE
	SOUND_LABEL_SE005,			// サンプルSE
	SOUND_LABEL_SE006,			// サンプルSE
	SOUND_LABEL_SE007,			// サンプルSE
	SOUND_LABEL_SE008,			// サンプルSE
	SOUND_LABEL_SE009,			// サンプルSE
	SOUND_LABEL_SE010,			// サンプルSE
	SOUND_LABEL_SE011,			// サンプルSE
	SOUND_LABEL_SE012,			// サンプルSE
	SOUND_LABEL_SE013,			// サンプルSE
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT XA_Initialize(void);
void XA_Release(void);
void XA_Play(SOUND_LABEL label);
void XA_Stop(SOUND_LABEL label);
void XA_Pause(SOUND_LABEL label);

class XA_BGM
{
public:
	XA_BGM(SOUND_LABEL label);
	~XA_BGM();
	void XA_Play_BGM(SOUND_LABEL label);
	void XA_Stop_BGM();
	void XA_Stop_BGM_Once();
	void XA_Play_BGM_OneMore();
	void XA_Play_SE(SOUND_LABEL label);

private:
	SOUND_LABEL labelBGM;
};
#endif
