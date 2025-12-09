#include "Audio.h"

namespace MyEngine {

	std::unique_ptr<Audio> Audio::instance_ = nullptr;

	// シングルトンインスタンスの取得
	Audio* Audio::GetInstance() {
		if (!instance_) {
			instance_ = std::make_unique<Audio>();
		}
		return instance_.get();
	}

	// 終了処理
	void Audio::Finalize() {
		// 読み込んだ全てのサウンドデータを解放
		for (auto& sound_data : loaded_sounds_) {
			SoundUnload(&sound_data);
		}

		// マスターボイスの破棄
		if (master_voice_ != nullptr) {
			master_voice_->DestroyVoice();
			master_voice_ = nullptr;
		}

		// XAudio2本体の解放
		x_audio2_.Reset();

		// シングルトンインスタンスの破棄
		instance_.reset();
	}

	// 初期化処理
	void Audio::Initialize() {
		// XAudio2エンジンを生成
		result_ = XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

		// マスターボイス（全体の音声出力管理）を作成
		result_ = x_audio2_->CreateMasteringVoice(&master_voice_);
	}

	// 音声データ（.wav）読み込み
	SoundData Audio::SoundLoadWave(const char* filename) {
		std::ifstream file;
		// WAVファイルをバイナリモードで開く
		file.open(filename, std::ios_base::binary);
		assert(file.is_open());

		// RIFFヘッダの読み込みとチェック
		RiffHeader riff;
		file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
		if (strncmp(riff.chunk.id, "RIFF", 4) != 0) { assert(0); }
		if (strncmp(riff.type, "WAVE", 4) != 0) { assert(0); }

		// Formatチャンクの読み込み
		FormatChunk format = {};
		file.read(reinterpret_cast<char*>(&format), sizeof(ChunkHeader));
		if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }
		assert(format.chunk.size <= sizeof(format.fmt));
		file.read(reinterpret_cast<char*>(&format.fmt), format.chunk.size);

		// Dataチャンクの探索と読み込み（JUNKチャンクがある場合はスキップ）
		ChunkHeader data;
		file.read(reinterpret_cast<char*>(&data), sizeof(data));
		if (strncmp(data.id, "JUNK", 4) == 0) {
			file.seekg(data.size, std::ios_base::cur);
			file.read(reinterpret_cast<char*>(&data), sizeof(data));
		}
		if (strncmp(data.id, "data", 4) != 0) { assert(0); }

		// 波形データ本体の読み込み
		char* p_buffer = new char[data.size];
		file.read(p_buffer, data.size);
		file.close();

		// 読み込んだデータをSoundData構造体にまとめる
		SoundData sound_data = {};
		sound_data.wfex = format.fmt;
		sound_data.p_buffer = reinterpret_cast<BYTE*>(p_buffer);
		sound_data.buffer_size = data.size;

		return sound_data;
	}

	// 音声データ解放
	void Audio::SoundUnload(SoundData* sound_data) {
		// バッファメモリの解放と構造体の初期化
		delete[] sound_data->p_buffer;
		sound_data->p_buffer = nullptr;
		sound_data->buffer_size = 0;
		sound_data->wfex = {};
	}

	// 音声再生
	void Audio::SoundPlayWave(const SoundData& sound_data) {
		// 読み込んだフォーマット情報をもとにSourceVoice（個別再生用ボイス）を生成
		IXAudio2SourceVoice* p_source_voice = nullptr;
		result_ = x_audio2_->CreateSourceVoice(&p_source_voice, &sound_data.wfex);
		assert(SUCCEEDED(result_));

		// 再生用バッファ情報の設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = sound_data.p_buffer;
		buf.AudioBytes = sound_data.buffer_size;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		// 再生開始
		result_ = p_source_voice->SubmitSourceBuffer(&buf);
		result_ = p_source_voice->Start();
	}
}