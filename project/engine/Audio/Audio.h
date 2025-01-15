#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <array>

#pragma comment(lib,"xaudio2.lib")

//チャンクヘッダー
struct ChunkHeader
{
	char id[4];//チャンク毎のID
	int32_t size;//チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk;//"RIFF"
	char type[4];//"WAVE"
};

//FMTチャンク
struct FormatChunk
{
	ChunkHeader chunk;//"fmt"
	WAVEFORMATEX fmt;//波型フォーマット
};

//音声データ
struct SoundData
{
	//波型フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
};

class Audio
{
public:
	static const int kMaxSound = 100;

	//シングルトンインスタンス
	static Audio* GetInstance();
	//終了
	void Finalize();

	void Initialize();
	//音声データ読み込み
	SoundData SoundLoadWave(const char* filename);
	//音声データ解放
	void SoundUnload(SoundData* soundData);
	//音声再生
	void SoundPlayWave(const SoundData& soundData);
private:
	static Audio* instance_;
	HRESULT result;
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	std::array<SoundData, kMaxSound>soundDatas_;
};

