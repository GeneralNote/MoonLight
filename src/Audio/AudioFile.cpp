#include <MoonLight/Audio/AudioFile.h>

namespace ml
{
	AudioFile::AudioFile() {
		m_loadThread = nullptr;
		m_owner = nullptr;
		m_loading = false;
		m_loaded = false;
		m_reader = nullptr;
	}
	AudioFile::~AudioFile() {
		if (m_loadThread) {
			delete m_loadThread;
			m_loadThread = nullptr;
		}

		if (m_reader) {
			m_reader->Release();
			m_reader = nullptr;
		}
	}

	void AudioFile::Load(const std::string& file, AudioEngine& engine)
	{
		m_loading = false;
		m_loaded = false;
		m_owner = nullptr;

		if (!engine.IsInitialized())
			return;

		m_owner = &engine;

		std::wstring wfile(file.begin(), file.end());

		if (m_reader) {
			m_reader->Release();
			m_reader = nullptr;
		}

		HRESULT hr = MFCreateSourceReaderFromURL(wfile.c_str(), engine.GetReaderConfiguration(), &m_reader);
		if (FAILED(hr))
			return;

		if (m_loadThread) {
			delete m_loadThread;
			m_loadThread = nullptr;
		}

		// TODO: m_loadThread = new std::thread(&AudioFile::m_load, this, wfile, m_reader);
		m_load(wfile, m_reader);
	}

	void AudioFile::m_load(const std::wstring& wfile, IMFSourceReader* reader)
	{
		m_loading = true;
		m_loaded = false;

		HRESULT hr;

		printf("jeff\n");
		hr = reader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
		if (FAILED(hr)) {
			m_loading = false;
			return;
		}

		hr = reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true);
		if (FAILED(hr)) {
			m_loading = false;
			return;
		}

		IMFMediaType* mediaType;
		hr = reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &mediaType);
		if (FAILED(hr)) {
			m_loading = false;
			return;
		}

		GUID majorType{};
		hr = mediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
		if (majorType != MFMediaType_Audio) {
			m_loading = false;
			return;
		}

		GUID subType{};
		hr = mediaType->GetGUID(MF_MT_MAJOR_TYPE, &subType);
		if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
		{
			// the audio file is already uncompressed
		}
		else
		{
			IMFMediaType* partialType = nullptr;
			hr = MFCreateMediaType(&partialType);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}

			hr = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}

			hr = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}

			hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, partialType);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}

			if (partialType)
				partialType->Release();
		}

		IMFMediaType* audioType = nullptr;
		hr = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &audioType);
		if (FAILED(hr)) {
			m_loading = false;
			return;
		}

		UINT32 waveLength = 0;
		hr = MFCreateWaveFormatExFromMFMediaType(audioType, &m_info, &waveLength);
		if (FAILED(hr)) {
			m_loading = false;
			return;
		}

		hr = reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true);
		if (FAILED(hr)) {
			m_loading = false;
			return;
		}

		IMFSample* sample = nullptr;
		IMFMediaBuffer* buffer = nullptr;
		BYTE* localAudioData = NULL;
		DWORD localAudioDataLength = 0;

		const int stepsInc = 1000000;
		int curDataInd = 0;
		m_samples.resize(stepsInc);

		while (true)
		{
			DWORD flags = 0;
			hr = reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}

			// check whether the data is still valid
			if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
				break;

			// check for eof
			if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				break;

			if (sample == nullptr)
				continue;

			hr = sample->ConvertToContiguousBuffer(&buffer);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}

			hr = buffer->Lock(&localAudioData, nullptr, &localAudioDataLength);
			if (FAILED(hr)) {
				m_loading = false;
				return;
			}


			if (curDataInd + localAudioDataLength > m_samples.size())
				m_samples.resize(m_samples.size() + stepsInc);

			for (size_t i = 0; i < localAudioDataLength; i++)
				m_samples[i + curDataInd] = localAudioData[i];
			curDataInd += localAudioDataLength;

			hr = buffer->Unlock();
			localAudioData = nullptr;

			if (FAILED(hr)) {
				m_loading = false;
				return;
			}
		}

		m_samples.resize(curDataInd);

		if (sample)
			sample->Release();
		if (buffer)
			buffer->Release();
		if (mediaType)
			mediaType->Release();

		m_loaded = true;
		m_loading = false;

		printf("%d\n", (int)m_loaded);

	}
}