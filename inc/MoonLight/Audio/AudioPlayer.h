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

		int SamplesPlayedCount();

	private:
		bool m_playing;
		IXAudio2SourceVoice* m_voice;
		AudioEngine* m_owner;
	};
}