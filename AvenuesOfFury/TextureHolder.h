#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder {
private:
	map<string, Texture> textureMap;
	static TextureHolder* thInstance;
public:
	TextureHolder();
	static Texture& getTexture(string const& filename);
};

#endif 