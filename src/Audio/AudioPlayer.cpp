#include <MoonLight/Audio/AudioPlayer.h>

namespace ml
{
	AudioPlayer::AudioPlayer()
	{
		m_voice = nullptr;
	}
	AudioPlayer::~AudioPlayer()
	{
		if (m_voice) {
			m_voice->DestroyVoice();
			m_voice = nullptr;
		}
	}

	bool AudioPlayer::Play(AudioFile& file, bool loop)
	{
		if (m_playing || file.IsLoading() || file.HasFailed())
			return false;

		m_owner = file.GetOwner();
		if (m_voice == nullptr)
			m_owner->GetEngine()->CreateSourceVoice(&m_voice, file.GetInfo());
		
		// create audio buffer
		XAUDIO2_BUFFER audioBuffer;
		ZeroMemory(&audioBuffer, sizeof(XAUDIO2_BUFFER));
		audioBuffer.AudioBytes = (UINT32)file.GetSamples().size();
		audioBuffer.pAudioData = (BYTE * const)&file.GetSamples()[0];
		audioBuffer.pContext = nullptr;

		if (loop)
			audioBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

		// submit the audio buffer
		HRESULT hr = m_voice->SubmitSourceBuffer(&audioBuffer);
		if (FAILED(hr))
			return false;

		// start playing
		m_voice->Start();
		m_playing = true;

	}
	void AudioPlayer::Stop()
	{
		if (!m_playing || m_voice == nullptr) return;

		m_voice->Stop();
	}
	int AudioPlayer::SamplesPlayedCount()
	{
		if (!m_playing)
			return 0;

		XAUDIO2_VOICE_STATE state;
		m_voice->GetState(&state);
		
		return state.SamplesPlayed;
	}
}