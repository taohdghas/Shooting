#pragma once

//リークを検出するクラス
class D3DResourceLeakChecker
{
public:
	/// <summary>
    /// デストラクタ。
	/// - プログラム終了時に DXGI のデバッグインターフェイスを取得してライブオブジェクトを報告することで
	///   Direct3D / DXGI リソースのリークを検出する。
	/// - コンストラクタは定義されておらず、スタックまたは静的領域で生成された場合に終了時自動でチェックが走ることを想定する。
	/// </summary>
	~D3DResourceLeakChecker();
};

