#pragma once
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace ml
{
	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		inline bool IsInitialized() { return m_initialized; }
		inline IXAudio2* GetEngine() { return m_engine; }
		inline IMFAttributes* GetReaderConfiguration() { return m_readerConf; }

	private:
		IMFAttributes* m_readerConf;
		IXAudio2* m_engine;
		IXAudio2MasteringVoice* m_master;

		bool m_initialized;
		bool m_init();
	};
}