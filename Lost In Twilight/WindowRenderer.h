#ifndef WINDOW_RENDERER
#define WINDOW_RENDERER

#include <SDL.h>
#include <map>
#include <string>

class WindowRenderer
{
public:
	WindowRenderer();
	void Present();
	void SetColor(int r, int g, int b, int transparency);
	void SetBackgroundColor(int r, int g, int b, int transparency);
	void DrawRectangle(int posx, int posy, int w, int h);
	void LoadTextureFromFile(const char* filename);
	void LoadTextureFromFile(const char* filename, double scale);
	void UnloadTexture(std::string);
	void ClearAllTextures();
	void DrawTexture(std::string filename, int x, int y);
	void DrawTexture(std::string filename, int x, int y, double degrees, SDL_RendererFlip flip);
	void DrawTexture(std::string filename, int xpos, int ypos, int x, int y, int w, int h, double degrees, double scale, SDL_RendererFlip flip);
	bool FindLoaded(std::string filename);
	void ToggleFullscreen();
	void SetAllSize(int w, int h);
	int GetWindowWidth();
	int GetWindowHeight();
	~WindowRenderer();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::map<std::string, SDL_Texture*> textureMap;
	int window_width;
	int window_height;
};

#endif