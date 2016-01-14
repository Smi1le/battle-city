#pragma once


struct music {
	SoundBuffer bufferShoot;
	SoundBuffer bufExplBullesWithBlocks;
	SoundBuffer bufExplNoDestroyObject;
	SoundBuffer bufExplEnemyOrPlayer;
	SoundBuffer bufStepCursor;

	Sound soundShoot;
	Sound soundExplBullesWithBlocks;
	Sound soundExplNoDestroyObject;
	Sound soundExplEnemyOrPlayer;
	Sound soundStepCursor;

	Music mainTheme;
	Music isMove;
	music() {
		bufferShoot.loadFromFile("recources/music/shoot.wav");
		bufExplBullesWithBlocks.loadFromFile("recources/music/explositionWithBlock.wav");
		bufExplNoDestroyObject.loadFromFile("recources/music/explOnNoDestroyObject.wav");
		bufExplEnemyOrPlayer.loadFromFile("recources/music/explEnemyOrPlayer.wav");
		bufStepCursor.loadFromFile("recources/music/soundStepCursorInMenu.wav");

		soundShoot.setBuffer(bufferShoot);
		soundExplBullesWithBlocks.setBuffer(bufExplBullesWithBlocks);
		soundExplNoDestroyObject.setBuffer(bufExplNoDestroyObject);
		soundExplEnemyOrPlayer.setBuffer(bufExplEnemyOrPlayer);
		soundStepCursor.setBuffer(bufStepCursor);

		mainTheme.openFromFile("recources/music/mainTheme.wav");
		isMove.openFromFile("recources/music/isMove.wav");
		
	}
	void volumeSounds(float volume) {
		soundShoot.setVolume(volume);
		soundExplBullesWithBlocks.setVolume(volume);
		soundExplNoDestroyObject.setVolume(volume);
		soundExplEnemyOrPlayer.setVolume(volume);
		soundStepCursor.setVolume(volume);

		mainTheme.setVolume(volume);
		isMove.setVolume(volume);
	}
};