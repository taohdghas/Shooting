#include "Audio.h"

Audio* Audio::instance_ = nullptr;

// シングルトンインスタンスの取得
Audio* Audio::GetInstance() {
	if (instance_ == nullptr)
	{
		instance_ = new Audio();
	}
	return instance_;
}

// 終了処理
void Audio::Finalize() {

	// 読み込んだ全てのサウンドデータを解放
	for (auto& soundData : loadedSounds_) {
		SoundUnload(&soundData);
	}

	// マスターボイスの破棄
	if (masterVoice_ != nullptr) {
		masterVoice_->DestroyVoice();
		masterVoice_ = nullptr;
	}

	// XAudio2本体の解放
	xAudio2_.Reset();

	delete instance_;
	instance_ = nullptr;
}

// 初期化処理
void Audio::Initialize() {
	// XAudio2エンジンを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイス（全体の音声出力管理）を作成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

// 音声データ（.wav）読み込み
SoundData Audio::SoundLoadWave(const char* filename)
{
	std::ifstream file;
	// WAVファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());

	// RIFFヘッダの読み込みとチェック
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) { assert(0); }
	if (strncmp(riff.type, "WAVE", 4) != 0) { assert(0); }

	// Formatチャンクの読み込み
	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの探索と読み込み（JUNKチャンクがある場合はスキップ）
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0) { assert(0); }

	// 波形データ本体の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	// 読み込んだデータをSoundData構造体にまとめる
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

// 音声データ解放
void Audio::SoundUnload(SoundData* soundData) {
	// バッファメモリの解放と構造体の初期化
	delete[] soundData->pBuffer;
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

// 音声再生
void Audio::SoundPlayWave(const SoundData& soundData) {
	// 読み込んだフォーマット情報をもとにSourceVoice（個別再生用ボイス）を生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生用バッファ情報の設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 再生開始
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}
