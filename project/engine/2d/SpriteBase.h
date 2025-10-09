#pragma once
#include "DirectXBase.h"
#include "Pso.h"

//スプライト共通部クラス
class SpriteBase
{
public:
	//シングルトンインスタンス
	static SpriteBase* GetInstance();
	//初期化
	void Initialize(DirectXBase*directxBase);
	//終了
	void Finalize();
	//共通描画設定
	void DrawBaseSet();
public:
	DirectXBase* GetDxBase()const { return directxBase_; }
private:
	//コンストラクタ
	SpriteBase() = default;
	//デストラクタ
	~SpriteBase() = default;
private:
	static SpriteBase* instance;
	SpriteBase* spritebase_ = nullptr;
	//コピーコンストラクタを無効にする
	SpriteBase(const SpriteBase&) = delete;
	//代入演算子を無効にする
	SpriteBase& operator = (const SpriteBase&) = delete;

	//DirectXBase
	DirectXBase* directxBase_;
	//Pso
	std::unique_ptr<Pso>pso_;
};

