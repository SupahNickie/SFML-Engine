#include "pch.h"
#include "Engine.h"
#include "Globals.h"

using namespace sf;

Engine::Engine() {
	g.setResolution(VideoMode::getDesktopMode().width / 2.0f, VideoMode::getDesktopMode().height / 2.0f);
	window.create(VideoMode(g.getResolution().x, g.getResolution().y), "Avenues of Fury", Style::Resize);

	//g.setResolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	//window.create(VideoMode(g.getResolution().x, g.getResolution().y), "Avenues of Fury", Style::Fullscreen);

	mainView.setSize(g.getResolution());
}

void Engine::run() {
	Clock clock;
	
	players = new PlayerCharacter*[2];
	players[0] = new Link;
	++numPlayers;
	players[0]->setPosition(Vector2f(100, 300));
	players[1] = new Skate;
	++numPlayers;
	players[1]->setPosition(Vector2f(250, 400));

	enemies = new EnemyCharacter*[100];
	enemies[0] = new Garnet(players);
	++numEnemies;
	enemies[0]->setPosition(Vector2f(500, 400));

	while (window.isOpen()) {
		Time dt = clock.restart();
		float dtAsSeconds = dt.asSeconds();
		input();
		update(dtAsSeconds);
		draw();
	}
}