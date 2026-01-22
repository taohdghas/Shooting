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
	struct ChunkHeader {
		char id[4];        // チャンク毎のID
		int32_t size;      // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4];      // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt;  // 波型フォーマット
	};

	// 音声データ
	struct SoundData {
		WAVEFORMATEX wfex;     // 波型フォーマット
		std::vector<BYTE> buffer;    // 音声データバッファ
	};

	// オーディオクラス
	class Audio {
	public:
		/// <summary>
		/// シングルトンインスタンスを取得する。
		/// </summary>
		static Audio* GetInstance();

		/// <summary>
		/// オーディオシステムを終了・解放する。
		/// </summary>
		void Finalize();

		/// <summary>
		/// オーディオシステムを初期化する。
		/// </summary>
		void Initialize();

		/// <summary>
		/// WAV ファイルを読み込み、SoundData を構築して返す。
		/// </summary>
		SoundData SoundLoadWave(const char* filename);

		/// <summary>
		/// SoundData が保持するバッファ等のリソースを解放する。
		/// </summary>
		void SoundUnload(SoundData* sound_data);

		/// <summary>
		/// 指定したサウンドデータを再生する。
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