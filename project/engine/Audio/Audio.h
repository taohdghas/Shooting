#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <vector>

#pragma comment(lib,"xaudio2.lib")

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
	BYTE* p_buffer;        // バッファの先頭アドレス
	unsigned int buffer_size; // バッファのサイズ
};

// オーディオクラス
class Audio {
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// プロセス内で単一の Audio インスタンスを返す。
	/// </summary>
	static Audio* GetInstance();

	/// <summary>
	/// オーディオシステムを終了・解放する。
	/// - マスター ボイスや XAudio2 インスタンス、ロード済みのサウンドを解放する。
	/// </summary>
	void Finalize();

	/// <summary>
	/// オーディオシステムを初期化する。
	/// - XAudio2 の初期化とマスターボイスの作成を行うことを想定する。
	/// </summary>
	void Initialize();

	/// <summary>
	/// WAV ファイルを読み込み、SoundData を構築して返す。
	/// - 指定したファイルを開き、RIFF/FMT/データチャンクを解析してバッファを確保する。
	/// </summary>
	/// <param name="filename">読み込む WAV ファイルのパス。</param>
	/// <returns>読み込んだ音声データを格納した <c>SoundData</c>。失敗時は適切な空のデータを返す実装が想定される。</returns>
	SoundData SoundLoadWave(const char* filename);

	/// <summary>
	/// SoundData が保持するバッファ等のリソースを解放する。
	/// - SoundLoadWave によって確保されたメモリを開放し、内部の管理リストから除去する等を行う。
	/// </summary>
	/// <param name="sound_data">解放するサウンドデータへのポインタ。</param>
	void SoundUnload(SoundData* sound_data);

	/// <summary>
	/// 指定したサウンドデータを再生する。
	/// - サンプルデータを XAudio2 のソースボイスへ送信して再生を開始する処理を想定する。
	/// </summary>
	/// <param name="sound_data">再生する音声データへの参照。</param>
	void SoundPlayWave(const SoundData& sound_data);

private:
	static Audio* instance_;
	HRESULT result_;
	Microsoft::WRL::ComPtr<IXAudio2> x_audio2_;
	IXAudio2MasteringVoice* master_voice_ = nullptr;
	std::vector<SoundData> loaded_sounds_;
};

