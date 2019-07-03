#pragma once
#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <windows.h>
#include <mmreg.h>

#include <MoonLight/Audio/AudioEngine.h>

namespace ml
{
	class AudioFile
	{
	public:
		AudioFile();
		~AudioFile();

		void Load(const std::string& file, AudioEngine& engine);

		inline std::vector<BYTE>& GetSamples() { return m_samples; }
		inline WAVEFORMATEX* GetInfo() { return m_info; }
		inline bool IsLoading() { return m_loading > 0; }
		inline bool HasFailed() { return !m_loaded && !m_loading; }
		inline AudioEngine* GetOwner() { return m_owner; }
		inline int GetTotalSamplesPerChannel() { return m_samples.size() / m_info->nChannels / (m_info->wBitsPerSample / 8); }
	
		inline int16_t GetSample(int i, bool skipSecondChannel = true) {
			i *= (m_info->wBitsPerSample / 8);
			if (skipSecondChannel)
				i *= m_info->nChannels;
			return ((m_samples[i]) | (m_samples[i + 1] << 8));
		}
		inline void Finalize() { if (!m_loading && m_loadThread && m_loadThread->joinable()) m_loadThread->join(); }
	private:
		void m_load(const std::wstring& file, IMFSourceReader* reader);

		AudioEngine* m_owner;
		IMFSourceReader* m_reader;

		std::thread* m_loadThread;
		std::atomic<bool> m_loading;
		std::atomic<bool> m_loaded;
		WAVEFORMATEX* m_info;
		std::vector<BYTE> m_samples;
	};
}