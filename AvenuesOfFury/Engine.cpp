#include "pch.h"
#include "Engine.h"
#include "Globals.h"
#include <iostream>

using namespace sf;

Engine::Engine() {
	g.setResolution(VideoMode::getDesktopMode().width / 2.0f, VideoMode::getDesktopMode().height / 2.0f);
	window.create(VideoMode(g.getResolution().x, g.getResolution().y), "Avenues of Fury", Style::Resize);

//	g.setResolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
//	window.create(VideoMode(g.getResolution().x, g.getResolution().y), "Avenues of Fury", Style::Fullscreen);

	mainView.setSize(g.getResolution());
}

void Engine::run() {
	Clock clock;
	srand(time(NULL));
	
	players = vector<PlayerCharacter*>(2);
	Link link;
	players[0] = &link;
	players[0]->setPosition(Vector2f(100, 300));
	Skate skate;
	players[1] = &skate;
	players[1]->setPosition(Vector2f(800, 400));

	enemies = vector<EnemyCharacter*>();
	Garnet garnet(players);
	enemies.push_back(&garnet);
	enemies[0]->setPosition(Vector2f(500, 400));
	Garnet garnet2(players);
	enemies.push_back(&garnet2);
	enemies[1]->setPosition(Vector2f(700, 300));
	Garnet garnet3(players);
	enemies.push_back(&garnet3);
	enemies[2]->setPosition(Vector2f(300, 100));

	while (window.isOpen()) {
		Time dt = clock.restart();
		float dtAsSeconds = dt.asSeconds();
//		cout << "TIME: " << dtAsSeconds * 1000 << "\n";
		input();
		update(dtAsSeconds);
		draw();
	}
}