#include "CDraw.h"

// static変数初期化
bool CDraw::isClear = false;
Float2 CDraw::camera = Float2zero;

CDraw::CDraw(const char *textureName, int maxSprite) {
	// とりあえず初期化失敗にしておく
	this->successInit = false;
	// とりあえず描画なし
	this->drawCount = 0;

#pragma region // 頂点バッファ作成 & テクスチャ読み込み (ほぼDX21まま)

	HRESULT hr;

	this->maxSprite = maxSprite;

	if (this->maxSprite <= 0) {
		return;
	}
	else if (this->maxSprite > MAX_SPRITE) {
		this->maxSprite = MAX_SPRITE;
	}

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	// ここは変更
	// MAX_SPRITEからthis->maxSpriteになっている
	// このクラスで使いたい数を指定するようにしておく
	bufferDesc.ByteWidth = this->maxSprite * sizeof(VERTEX_POSTEX) * VERTEX_PER_SPRITE;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Direct3D_GetDevice()->CreateBuffer(&bufferDesc, NULL, &this->pVertexBuffer);

	if (FAILED(hr)) {
		return;
	}
	// ↑　頂点バッファ作成　ここまで

	// ここで使用したい画像ファイルをテクスチャとして読み込む
	// 第一引数：画像ファイル名。もしフォルダに入っているならフォルダ名も一緒に書く。
	// 第二引数：読み込んだテクスチャが入る変数を指定
	hr = Direct3D_LoadTexture(textureName, &this->pTexture);

	if (FAILED(hr)) {
		return;
	}

#pragma endregion

	// 初期化成功
	this->successInit = true;
}

CDraw::~CDraw() {
	COM_SAFE_RELEASE(pTexture);  // テクスチャを読み込んだら、忘れずリリースすること
	COM_SAFE_RELEASE(pVertexBuffer); // 頂点バッファを作成したら、忘れずにリリースすること
}

void CDraw::Update() {
	// 画面クリアをできるように
	CDraw::isClear = false;
}

void CDraw::SetVertex(CGameObject *drawTarget) {

	// 初期化不十分ならする必要性なし
	// カウントが最大値に達しているならそれ以上格納できない
	if (this->successInit && this->drawCount < this->maxSprite) {
		// 各頂点の座標とUV座標計算
		FRECT verPos = drawTarget->GameObject_GetXY();
		FRECT verUV = drawTarget->GameObject_GetUV();

		verPos.left -= this->camera.x / SCREEN_WIDTH;
		verPos.right -= this->camera.x / SCREEN_WIDTH;
		verPos.top -= this->camera.y / SCREEN_HEIGHT;
		verPos.bottom -= this->camera.y / SCREEN_HEIGHT;

		// 配列に格納
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 0] = { verPos.left, verPos.top, 0.0f, verUV.left, verUV.top };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 1] = { verPos.right, verPos.top, 0.0f, verUV.right, verUV.top };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 2] = { verPos.right, verPos.bottom, 0.0f, verUV.right, verUV.bottom };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 3] = { verPos.right, verPos.bottom, 0.0f, verUV.right, verUV.bottom };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 4] = { verPos.left, verPos.bottom, 0.0f, verUV.left, verUV.bottom };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 5] = { verPos.left, verPos.top, 0.0f, verUV.left, verUV.top };

		// 描画数カウントを増やす
		this->drawCount++;
	}
}

void CDraw::SetVertex(CGameObject *drawTarget, Float2 camera) {

	// 初期化不十分ならする必要性なし
	// カウントが最大値に達しているならそれ以上格納できない
	if (this->successInit && this->drawCount < this->maxSprite) {
		// 各頂点の座標とUV座標計算
		FRECT verPos = drawTarget->GameObject_GetXY();
		FRECT verUV = drawTarget->GameObject_GetUV();

		verPos.left -= camera.x / SCREEN_WIDTH;
		verPos.right -= camera.x / SCREEN_WIDTH;
		verPos.top -= camera.y / SCREEN_HEIGHT;
		verPos.bottom -= camera.y / SCREEN_HEIGHT;

		// 配列に格納
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 0] = { verPos.left, verPos.top, 0.0f, verUV.left, verUV.top };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 1] = { verPos.right, verPos.top, 0.0f, verUV.right, verUV.top };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 2] = { verPos.right, verPos.bottom, 0.0f, verUV.right, verUV.bottom };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 3] = { verPos.right, verPos.bottom, 0.0f, verUV.right, verUV.bottom };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 4] = { verPos.left, verPos.bottom, 0.0f, verUV.left, verUV.bottom };
		this->vx[this->drawCount * VERTEX_PER_SPRITE + 5] = { verPos.left, verPos.top, 0.0f, verUV.left, verUV.top };

		// 描画数カウントを増やす
		this->drawCount++;
	}
}

void CDraw::DrawAct() {

	// 初期化不十分なら通ってはならない！
	// 絶対に！
	if (this->successInit) {
		// 頂点バッファのデータをvx配列のデータで更新する
		Direct3D_GetContext()->UpdateSubresource(
			this->pVertexBuffer,		// 更新対象のバッファ
			0,				// インデックス(0)
			NULL,				// 更新範囲(nullptr)
			vx,		// 反映データ
			0,				// データの1行のサイズ(0)
			0);				// 1深度スライスのサイズ(0)

		// ↓　自前の描画処理をここに書く *******

		// クリア済みフラグが立っていないなら画面クリアする
		if (!this->isClear) {
			// 画面クリア（指定色で塗りつぶし）
			// 塗りつぶし色の指定（小数：0.0f～1.0f）
			float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

			Direct3D_GetContext()->ClearRenderTargetView(Direct3D_GetRenderTargetView(), clearColor);

			// クリア済みフラグを立てる
			this->isClear = true;
		}

		// 描画で使用するテクスチャを指定する関数呼び出し
		// ここで指定されたテクスチャが、ピクセルシェーダーのグローバル変数にセットされる
		Direct3D_GetContext()->PSSetShaderResources(0, 1, &this->pTexture);

		// 描画に使う頂点バッファを指定する
		UINT strides = sizeof(VERTEX_POSTEX);
		UINT offsets = 0;
		Direct3D_GetContext()->IASetVertexBuffers(0, 1, &this->pVertexBuffer, &strides, &offsets);   // gpVertexBufferが頂点バッファ

		// これまでの設定で実際に描画する
		// 描画数はthis->drawCount
		// これにより描画時のバグを軽減する
		Direct3D_GetContext()->Draw(this->drawCount *VERTEX_PER_SPRITE, 0);

		// 描画を終了したので描画数を0にする
		this->drawCount = 0;

		// ↑　自前の描画処理をここに書く *******
	}
}

