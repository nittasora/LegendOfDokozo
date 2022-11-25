//=============================================================================
//
// �T�E���h���� [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// �T���v��BGM
	SOUND_LABEL_BGM001,			// �T���v��BGM
	SOUND_LABEL_BGM002,			// �T���v��BGM
	SOUND_LABEL_BGM003,			// �T���v��BGM
	SOUND_LABEL_BGM004,			// �T���v��BGM
	SOUND_LABEL_BGM005,			// �T���v��BGM
	SOUND_LABEL_BGM006,			// �T���v��BGM
	SOUND_LABEL_SE000,			// �T���v��SE
	SOUND_LABEL_SE001,			// �T���v��SE
	SOUND_LABEL_SE002,			// �T���v��SE
	SOUND_LABEL_SE003,			// �T���v��SE
	SOUND_LABEL_SE004,			// �T���v��SE
	SOUND_LABEL_SE005,			// �T���v��SE
	SOUND_LABEL_SE006,			// �T���v��SE
	SOUND_LABEL_SE007,			// �T���v��SE
	SOUND_LABEL_SE008,			// �T���v��SE
	SOUND_LABEL_SE009,			// �T���v��SE
	SOUND_LABEL_SE010,			// �T���v��SE
	SOUND_LABEL_SE011,			// �T���v��SE
	SOUND_LABEL_SE012,			// �T���v��SE
	SOUND_LABEL_SE013,			// �T���v��SE
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
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
