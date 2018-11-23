#include "pch.h"
#include "Engine.h"

Engine::Engine() {
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width / 2.0f;
	resolution.y = VideoMode::getDesktopMode().height / 2.0f;
	window.create(VideoMode(resolution.x, resolution.y), "Avenues of Fury", Style::Resize);

	mainView.setSize(resolution);
}

void Engine::run() {
	Clock clock;
	player1.setSecondPlayer();
	player2.setSecondPlayer();
	while (window.isOpen()) {
		Time dt = clock.restart();
		float dtAsSeconds = dt.asSeconds();
		input();
		update(dtAsSeconds);
		draw();
	}
}