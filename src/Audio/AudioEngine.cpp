#include <MoonLight/Audio/AudioEngine.h>

#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid")

namespace ml
{
	AudioEngine::AudioEngine()
	{
		m_initialized = m_init();
	}

	AudioEngine::~AudioEngine()
	{
		m_initialized = false;

		// clean up
		m_master->DestroyVoice();
		m_engine->StopEngine();

		if (m_readerConf) {
			m_readerConf->Release();
			m_readerConf = nullptr;
		}
		if (m_engine) {
			m_engine->Release();
			m_engine = nullptr;
		}

		MFShutdown();
	}

	bool AudioEngine::m_init()
	{

		HRESULT hr;

		hr = MFStartup(MF_VERSION);
		if (FAILED(hr))
			return false;

		hr = MFCreateAttributes(&m_readerConf, 1);
		if (FAILED(hr))
			return false;

		hr = m_readerConf->SetUINT32(MF_LOW_LATENCY, true);
		if (FAILED(hr))
			return false;

		hr = XAudio2Create(&m_engine);
		if (FAILED(hr))
			return false;

		hr = m_engine->CreateMasteringVoice(&m_master);
		if (FAILED(hr))
			return false;
		
		return true;
	}
}