#include <MoonLight/Audio/AudioEngine.h>
#include <MoonLight/Audio/AudioFile.h>
#include <MoonLight/Audio/AudioPlayer.h>

int main()
{
	ml::AudioEngine engine;
	ml::AudioFile file;
	ml::AudioPlayer player;

	file.Load("test.wav", engine);

	printf("Waiting for file to load...\n");

	// wait until thread for loading the file finishes
	while (file.IsLoading()) continue;

	if (file.HasFailed()) {
		printf("Failed to load the audio file.\n");
		return 0;
	}

	printf("Loaded!\n");

	player.Play(file, true);

	while (true) {
		char c = getchar();
		if (c == 'q' || c == 'Q')
			break;
	}
	
	player.Stop();

	return 0;
}