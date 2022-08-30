#include "Background.h"

Background::Background() {}

Background::Background(int width, int height, int scale) {

	srcRect.x = srcRect.y = 0;
	srcRect.w = width;
	srcRect.h = height;
	backgroundScale = scale;
}

void Background::update() {

	for (auto& c : wallpapers) {

		c.destRect1.x -= c.wallpaperSpeed;
		c.destRect2.x -= c.wallpaperSpeed;

		if (c.destRect1.x < -c.destRect1.w)
			c.destRect1.x = c.destRect2.x + c.destRect2.w;

		else if (c.destRect2.x < -c.destRect2.w)
			c.destRect2.x = c.destRect1.x + c.destRect1.w;
	}
}

void Background::draw() {

	for (auto& c : wallpapers) {

		TextureManager::DrawTexture(c.wallpaperTexture, srcRect, c.destRect1);
		TextureManager::DrawTexture(c.wallpaperTexture, srcRect, c.destRect2);
	}
}

void Background::addWallpaper(const char* path, int speed) {

	Wallpaper wallp;
	wallp.destRect1.x = 0;
	wallp.destRect1.y = 0;
	wallp.destRect2.y = 0;
	wallp.wallpaperSpeed = speed;
	wallp.destRect1.w = srcRect.w * backgroundScale;
	wallp.destRect1.h = srcRect.h * backgroundScale;
	wallp.destRect2.w = srcRect.w * backgroundScale;
	wallp.destRect2.h = srcRect.h * backgroundScale;
	wallp.destRect2.x = wallp.destRect1.w;
	wallp.wallpaperTexture = TextureManager::LoadTexture(path);
	wallpapers.push_back(wallp);
}