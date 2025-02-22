#include "WindowRenderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "GlobalVariables.h"

WindowRenderer::WindowRenderer()
{
	IMG_Init(IMG_INIT_PNG);
	window_width = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;
	window = SDL_CreateWindow("Lost In Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	window_width = 1280;
	window_height = 720;
	SDL_SetWindowSize(window, window_width, window_height);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	//View Outside
	//SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
}

void WindowRenderer::Present()
{
	SDL_RenderPresent(renderer);
}

void WindowRenderer::SetColor(int r, int g, int b, int transparency)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, transparency);
}

void WindowRenderer::SetBackgroundColor(int r, int g, int b, int transparency) {
	SDL_SetRenderDrawColor(renderer, r, g, b, transparency);
	SDL_RenderClear(renderer);
}

void WindowRenderer::DrawRectangle(int posx, int posy, int w, int h) {
	SDL_Rect rect;
	rect.x = posx;
	rect.y = posy;
	rect.w = w;
	rect.h = h;
	SDL_RenderFillRect(renderer, &rect);
}

void WindowRenderer::LoadTextureFromFile(const char* filename)
{
	if (!FindLoaded(filename)) {
		textureMap[filename] = SDL_CreateTextureFromSurface(renderer, IMG_Load(filename));
	}
}

void WindowRenderer::LoadTextureFromFile(const char* filename, double scale)
{
	SDL_Surface* surface = IMG_Load(filename);
	SDL_Surface* newSurface = SDL_CreateRGBSurfaceWithFormat(0, surface->w * scale, surface->h * scale, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_BlitScaled(surface, nullptr, newSurface, nullptr);
	textureMap[filename] = SDL_CreateTextureFromSurface(renderer, newSurface);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(newSurface); 
}

void WindowRenderer::UnloadTexture(std::string filename)
{
	if (textureMap.find(filename) != textureMap.end()) {
		SDL_DestroyTexture(textureMap[filename]);
		textureMap.erase(filename);
	}
}

void WindowRenderer::ClearAllTextures()
{
	for (auto i = textureMap.begin(); i != textureMap.end(); i++) {
		SDL_DestroyTexture(i->second);
	}
	textureMap.clear();
}

void WindowRenderer::DrawTexture(std::string filename, int x, int y)
{
	int w;
	int h;
	SDL_QueryTexture(textureMap[filename], NULL, NULL, &w, &h);
	SDL_Rect srect;
	SDL_Rect drect;
	srect.x = 0; srect.y = 0; srect.w = w; srect.h = h;
	drect.x = x - w / 2; drect.y = y - h / 2; drect.w = w; drect.h = h;

	SDL_RenderCopy(renderer, textureMap[filename], &srect, &drect);
}

void WindowRenderer::DrawTexture(std::string filename, int x, int y, double degrees, SDL_RendererFlip flip)
{
	int w;
	int h;
	SDL_QueryTexture(textureMap[filename], NULL, NULL, &w, &h);

	SDL_Rect srect;
	SDL_FRect drect;
	srect.x = 0; srect.y = 0; srect.w = w; srect.h = h;
	drect.x = x - w / 2; drect.y = y - h / 2; drect.w = w; drect.h = h;

	SDL_FPoint p;
	p.x = w / 2;
	p.y = h / 2;

	SDL_RenderCopyExF(renderer, textureMap[filename], &srect, &drect, degrees, &p, flip);
}

void WindowRenderer::DrawTexture(std::string filename, int xpos, int ypos, int x, int y, int w, int h, double degrees, double scale, SDL_RendererFlip flip)
{
	x *= scale;
	y *= scale;
	w *= scale;
	h *= scale;
	SDL_Rect srect;
	SDL_FRect drect;
	srect.x = x; srect.y = y; srect.w = w; srect.h = h;
	drect.x = xpos - w / 2; drect.y = ypos - h / 2; drect.w = w; drect.h = h;

	SDL_FPoint p;
	p.x = w / 2;
	p.y = h / 2;

	SDL_RenderCopyExF(renderer, textureMap[filename], &srect, &drect, degrees, &p, flip);
}

bool WindowRenderer::FindLoaded(std::string filename)
{
	return textureMap.find(filename) != textureMap.end();
}

void WindowRenderer::ToggleFullscreen()
{
	SDL_SetWindowFullscreen(window, (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) ? 0 : SDL_WINDOW_FULLSCREEN);
}

int WindowRenderer::GetWindowWidth()
{
	return window_width;
}

int WindowRenderer::GetWindowHeight()
{
	return window_height;
}

void WindowRenderer::ViewHitbox(int xpos, int ypos, int w, int h)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
	DrawRectangle(xpos, ypos, w, h);
}

void WindowRenderer::ViewInverseHitbox(int xpos, int ypos, int w, int h)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
	DrawRectangle(xpos - 5, ypos, 5, h);
	DrawRectangle(xpos - 5, ypos - 5, w + 10, 5);
	DrawRectangle(xpos + w, ypos, 5, h);
	DrawRectangle(xpos - 5, ypos + h, w + 10, 5);
}

WindowRenderer::~WindowRenderer()
{
	ClearAllTextures();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
}
