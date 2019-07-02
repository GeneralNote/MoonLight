#include <MoonLight/Audio/AudioEngine.h>
#include <MoonLight/Audio/AudioFile.h>
#include <MoonLight/Audio/AudioPlayer.h>
#include <SFML/Audio.hpp>

int main()
{
	ml::AudioEngine engine;
	ml::AudioFile file;
	ml::AudioPlayer player;

	file.Load("test.wav", engine);

	printf("Waiting for file to load...\n");

	// wait until thread for loading the file finishes
	while (file.IsLoading()) { }

	// must call this after we finish loading an audio file
	printf("Finishing up!\n");
	file.Prepare();

	if (file.HasFailed()) {
		printf("Failed to load the audio file.\n");
		return 0;
	}

	printf("Loaded!\n");


	sf::SoundBuffer buf;
	buf.loadFromFile("test.wav");


	player.Play(file, true);

	while (true) {
		char c = getchar();
		if (c == 'q' || c == 'Q')
			break;

		int index = player.SamplesPlayedCount();

		if (buf.getSamples()[index] != file.GetSample(index))
			printf("jeff!\n");
	}
	
	player.Stop();

	return 0;
}