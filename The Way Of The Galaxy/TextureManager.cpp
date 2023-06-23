#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* path) {

	// Return a texture of an image
	SDL_Surface* textureSurface = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::renderer, textureSurface);
	SDL_FreeSurface(textureSurface);
	return texture;
}

SDL_Texture* TextureManager::LoadTexture(TTF_Font* font, SDL_Color color, const char* text) {

	// Return a texture of a text
	SDL_Surface* textureSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::renderer, textureSurface);
	SDL_FreeSurface(textureSurface);
	return texture;
}

void TextureManager::DrawTexture(SDL_Texture* tex, SDL_Rect destRect) {

	SDL_RenderCopy(Engine::renderer, tex, NULL, &destRect);
}

void TextureManager::DrawTexture(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect) {

	SDL_RenderCopy(Engine::renderer, tex, &srcRect, &destRect);
}

void TextureManager::DrawTexture(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip) {

	SDL_RenderCopyEx(Engine::renderer, tex, &srcRect, &destRect, 0, NULL, flip);
}

void TextureManager::DrawTexture(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle) {

	SDL_RenderCopyEx(Engine::renderer, tex, &srcRect, &destRect, angle, NULL, flip);
}