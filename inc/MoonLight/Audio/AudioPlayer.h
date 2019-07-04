#pragma once
#include <MoonLight/Audio/AudioFile.h>

namespace ml
{
	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();

		bool Play(AudioFile& file, bool loop = false);
		void Stop();

		inline void SetVolume(float vol) { if (m_voice) m_voice->SetVolume(vol); }
		inline float GetVolume() { float ret; m_voice->GetVolume(&ret); return ret; }

		inline bool IsPlaying() { return m_playing; }
		int SamplesPlayedCount();

	private:
		bool m_playing;
		IXAudio2SourceVoice* m_voice;
		AudioEngine* m_owner;
	};
}