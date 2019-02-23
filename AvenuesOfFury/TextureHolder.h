#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace sf;
using namespace std;

class TextureHolder {
public:
	TextureHolder();
	static void deleteTexture(string const& filename);
	static Texture& getTexture(string const& filename);
private:
	unordered_map<string, Texture> textureMap;
	static TextureHolder* thInstance;
};

#endif 