#include "pch.h"
#include <assert.h>
#include "TextureHolder.h"

using namespace sf;
using namespace std;

TextureHolder* TextureHolder::thInstance = nullptr;

TextureHolder::TextureHolder() {
	assert(thInstance == nullptr);
	thInstance = this;
}

Texture& TextureHolder::getTexture(string const& filename) {
	auto& tMap = thInstance->textureMap;
	auto textureEntry = tMap.find(filename);
	if (textureEntry != tMap.end()) {
		return textureEntry->second;
	}
	else {
		Texture& texture = tMap[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}