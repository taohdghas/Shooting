#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <vector>
#include <memory>

#pragma comment(lib,"xaudio2.lib")

namespace MyEngine {

	// チャンクヘッダー
	/// <summary>
	/// WAVファイルの各チャンクのIDとサイズを保持する構造体
	/// </summary>
	struct ChunkHeader {
		char id[4];        // チャンク毎のID
		int32_t size;      // チャンクサイズ
	};

	// RIFFヘッダチャンク
	/// <summary>
	/// WAVファイルのRIFFヘッダチャンク情報を保持する構造体
	/// </summary>
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4];      // "WAVE"
	};

	// FMTチャンク
	/// <summary>
	/// WAVファイルのフォーマット（fmt）チャンク情報を保持する構造体
	/// </summary>
	struct FormatChunk {
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt;  // 波型フォーマット
	};

	// 音声データ
	/// <summary>
	/// 波形フォーマットと音声データバッファを保持する構造体
	/// </summary>
	struct SoundData {
		WAVEFORMATEX wfex;     // 波型フォーマット
		std::vector<BYTE> buffer;    // 音声データバッファ
	};

	// オーディオクラス
    /// <summary>
    /// XAudio2を用いたオーディオシステムの初期化・終了処理
    /// WAVファイルの読み込みとサウンドデータの管理
    /// サウンドデータの再生・解放処理
    /// シングルトンによるインスタンス管理
    /// XAudio2リソースの管理
    /// </summary>
	class Audio {
	public:
		/// <summary>
        /// シングルトンインスタンスを取得
        /// <returns>Audioのインスタンス（ポインタ）</returns>
        /// </summary>
		static Audio* GetInstance();

		/// <summary>
		/// オーディオシステムを終了・解放
		/// <returns>なし</returns>
		/// </summary>
		void Finalize();

		/// <summary>
		/// オーディオシステムを初期化
		/// <returns>なし</returns>
		/// </summary>
		void Initialize();

		/// <summary>
		/// WAV ファイルを読み込み、SoundData を構築して返す
		/// <param name="filename">WAVファイルのパス</param>
		/// <returns>SoundData構造体</returns>
		/// </summary>
		SoundData SoundLoadWave(const char* filename);

		/// <summary>
		/// SoundData が保持するバッファ等のリソースを解放
		/// <param name="sound_data">解放対象のSoundData構造体のポインタ</param>
		/// <returns>なし</returns>
		/// </summary>
		void SoundUnload(SoundData* sound_data);

		/// <summary>
		/// 指定したサウンドデータを再生
		/// <param name="sound_data">再生するSoundData構造体</param>
		/// <returns>なし</returns>
		/// </summary>
		void SoundPlayWave(const SoundData& sound_data);

	public:
		Audio() = default;
		~Audio() = default;

	private:
		/// シングルトンインスタンス
		static std::unique_ptr<Audio> instance_;
		// XAudio2関連の結果コード
		HRESULT result_;
		// XAudio2本体
		Microsoft::WRL::ComPtr<IXAudio2> x_audio2_;
		// マスターボイス
		IXAudio2MasteringVoice* master_voice_ = nullptr;
		// 読み込んだサウンドデータのリスト
		std::vector<SoundData> loaded_sounds_;

		std::unique_ptr<BYTE[]>buffer_;
	};
}