#include "pch.h"
#include "Engine.h"

using namespace sf;

Engine::Engine() {
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width / 2.0f;
	resolution.y = VideoMode::getDesktopMode().height / 2.0f;
	window.create(VideoMode(resolution.x, resolution.y), "Avenues of Fury", Style::Resize);

	/*resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	window.create(VideoMode(resolution.x, resolution.y), "Avenues of Fury", Style::Fullscreen);*/

	mainView.setSize(resolution);
}

void Engine::run() {
	Clock clock;
	
	characters = new Character*[100];
	characters[0] = new Link;
	++numCharacters;
	characters[0]->setPosition(Vector2f(100, 300));
	characters[1] = new Skate;
	++numCharacters;
	characters[1]->setPosition(Vector2f(250, 400));
	characters[2] = new Garnet;
	++numCharacters;
	characters[2]->setPosition(Vector2f(500, 400));

	while (window.isOpen()) {
		Time dt = clock.restart();
		float dtAsSeconds = dt.asSeconds();
		input();
		update(dtAsSeconds);
		draw();
	}
}