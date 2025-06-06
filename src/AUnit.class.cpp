/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-24-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: AUnit.class.cpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#include "AUnit.class.hpp"

AUnit::AUnit(void) : _width(0),
					 _height(0),
					 _x(0),
					 _y(0),
					 _texture(NULL)
{
}

AUnit::~AUnit(void)
{
	free();
}

int AUnit::getWidth(void)
{
	return _width;
}

int AUnit::getHeight(void)
{
	return _height;
}

int AUnit::getX(void)
{
	return _x;
}

int AUnit::getY(void)
{
	return _y;
}

SDL_Texture *AUnit::getTexture(void)
{
	return _texture;
}

void AUnit::setWidth(int width)
{
	_width = width;
}

void AUnit::setHeight(int height)
{
	_height = height;
}

void AUnit::setX(int x)
{
	_x = x;
}

void AUnit::setY(int y)
{
	_y = y;
}

bool AUnit::load(std::string path, t_settings &settings)
{
	free();
	SDL_Texture *newTexture = NULL;
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		std::cout << "Failed to load image " << path << ": " << IMG_GetError() << std::endl;
		return false;
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x4F, 0x24, 0x7A));

	// Set dimensions before creating texture
	_width = surface->w;
	_height = surface->h;

	newTexture = SDL_CreateTextureFromSurface(settings.renderer, surface);
	if (newTexture == NULL)
	{
		std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return false;
	}

	SDL_FreeSurface(surface);
	_texture = newTexture;
	return true;
}

void AUnit::free(void)
{
	if (_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
		_texture = NULL;
	}
}

void AUnit::render(t_settings &settings)
{
	SDL_Rect rect = {_x, _y, _width, _height};
	SDL_RenderCopy(settings.renderer, _texture, NULL, &rect);
}

void AUnit::moveRight(void)
{
	if (_x >= WINDOW_WIDTH - PLAYER_WIDTH - 3)
		return;
	_x += 3;
}

void AUnit::moveLeft(void)
{
	if (_x <= 3)
		return;
	_x -= 3;
}

void AUnit::moveUp(void)
{
	if (_y <= 3)
		return;
	_y -= 3;
}
