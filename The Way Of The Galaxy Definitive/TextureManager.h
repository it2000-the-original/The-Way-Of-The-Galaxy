#pragma once
#include "SDL.h"
#include "Engine.h"

class TextureManager {

public:

	static SDL_Texture* LoadTexture(const char* path);
	static SDL_Texture* LoadTexture(TTF_Font* font, SDL_Color color, const char* text);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect destRect);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle);
};