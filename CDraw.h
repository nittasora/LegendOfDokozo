#pragma once

// 汎用性や見てくれの良さ等王先生サンプルに基本劣る。
// 参考にした誰かさんはシンプルにそのことについて覚えておいてもらいたい。

class CDraw;

#include "Information.h"	// 各種定数と構造体用(DX21サンプルより移動)
#include "CGameObject.h"	// 描画するオブジェクト用(DX21サンプル内"GameObject.h"を改変・追記、ただし互換性なし)

// 描画用クラス。
// 読み込むテクスチャの分生成してください。
class CDraw
{
public:
	// 機能:初期化
	// 引数1:読み込むテクスチャのパス(例) "assets/TitleBack.png")
	// 引数2:スプライトの最大数(ただしMAX_SPRITE以下)
	// 例) CDraw *pBG_Draw = new CDraw("assets/TitleBack.png", 2);
	CDraw(const char*, int);
	// 機能:解放処理
	// newを使っているので解放してあげなければいけない
	~CDraw();
	// 機能:isClearをfalseにする
	// 必ずゲームループ内の描画より先に呼び出さないといけない
	static void Update();
	// 機能:オブジェクトの頂点を格納
	// 引数:ゲームオブジェクト(DX21サンプルではGameObjectのこと)
	void SetVertex(CGameObject*);
	// 機能:オブジェクトの頂点を格納
	// 引数:ゲームオブジェクト(DX21サンプルではGameObjectのこと)
	void SetVertex(CGameObject*, Float2);
	// 機能:実際に描画
	void DrawAct();

	// 疑似カメラ
	static Float2 camera;

private:
	// 頂点バッファ用の変数
	ID3D11Buffer* pVertexBuffer;
	// テクスチャ用変数
	ID3D11ShaderResourceView* pTexture;
	// (原文ほぼまま)
	// ポリゴンの頂点を定義
	// 頂点を結んでポリゴンを形成するときの法則
	// ・頂点は、配列の番号の若い順番に使用される
	// ・頂点を結んでポリゴンが形成されたとき、その結び順が時計回りになる場合だけポリゴンが表示される
	VERTEX_POSTEX vx[MAX_SPRITE * VERTEX_PER_SPRITE];
	// 最大スプライト数
	int maxSprite;
	// 描画する数
	int drawCount;
	// 正常に初期化されたか
	bool successInit;
	// 一度だけ画面クリアするためのフラグ
	static bool isClear;
};

