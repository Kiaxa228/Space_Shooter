/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-25-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: main.cpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#include "SpaceShooter.hpp"
#include "Player.class.hpp"
#include "Background.class.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#undef main

bool init(t_settings &settings)
{
	std::cout << "Initializing SDL..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "Setting up window..." << std::endl;
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	settings.window = SDL_CreateWindow("Space Shooter",
									   SDL_WINDOWPOS_UNDEFINED,
									   SDL_WINDOWPOS_UNDEFINED,
									   WINDOW_WIDTH,
									   WINDOW_HEIGHT + INFOBOX_HEIGHT,
									   SDL_WINDOW_SHOWN);

	if (settings.window == NULL)
	{
		std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "Creating renderer..." << std::endl;
	settings.renderer = SDL_CreateRenderer(settings.window, -1, SDL_RENDERER_ACCELERATED);
	if (settings.renderer == NULL)
	{
		std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderDrawColor(settings.renderer, 0x61, 0x23, 0x7A, 0xFF);

	std::cout << "Initializing SDL_image..." << std::endl;
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
		return false;
	}

	std::cout << "Initializing SDL_ttf..." << std::endl;
	if (TTF_Init() == -1)
	{
		std::cout << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
		return false;
	}

	std::cout << "All SDL subsystems initialized successfully!" << std::endl;
	return true;
}

void close(t_settings &settings, Player &player)
{
	std::cout << "Cleaning up..." << std::endl;
	player.free();

	SDL_DestroyRenderer(settings.renderer);
	SDL_DestroyWindow(settings.window);
	settings.renderer = NULL;
	settings.window = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	std::cout << "Cleanup complete!" << std::endl;
}

extern "C" int main(int ac, char *av[])
{
	Background background;
	t_settings settings;
	srand(time(0));

	settings.window = NULL;
	settings.renderer = NULL;

	if (!init(settings))
	{
		std::cout << "Failed to initialize!" << std::endl;
		return 1;
	}

	Player player;
	if (!player.load(PLAYER_IMG, settings))
	{
		std::cout << "Failed to load Player image: " << PLAYER_IMG << std::endl;
		return 1;
	}

	bool quit = false;
	SDL_Event event;
	unsigned long ticks = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
				case SDLK_a:
					player.moveLeft();
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					player.moveRight();
					break;
				case SDLK_UP:
				case SDLK_w:
					player.moveUp();
					break;
				case SDLK_DOWN:
				case SDLK_s:
					player.moveDown();
					break;
				case SDLK_SPACE:
					player.shoot(settings);
					break;
				case SDLK_ESCAPE:
					quit = true;
				default:
					break;
				}
			}
		}

		if (ticks % (STAR_HEIGHT * 5) == 0)
			background.makeStar(settings);

		if (ticks % (METEOR_HEIGHT * 20) == 0)
			background.makeMeteor(settings);

		if (ticks % (ENEMY_HEIGHT * 24) == 0)
			background.makeEnemy(settings);

		background.killEnemy(player);
		background.killMeteor(player);

		if (background.hitEnemy(player) || background.hitMeteor(player))
		{
			SDL_SetRenderDrawColor(settings.renderer, 0x61, 0x23, 0x7A, 0xFF);
			SDL_RenderClear(settings.renderer);
			background.loadGameOver(GAME_OVER, settings);
			SDL_RenderPresent(settings.renderer);
			SDL_Delay(2000);
			break;
		}

		SDL_SetRenderDrawColor(settings.renderer, 0x61, 0x23, 0x7A, 0xFF);
		SDL_RenderClear(settings.renderer);

		background.displayStar(settings);
		background.displayMeteor(settings, (ticks % 2 == 0));
		background.displayEnemy(settings, (ticks % (ENEMY_HEIGHT / 2) == 0));

		player.render(settings);
		player.moveBullets(settings);

		background.drawInfoBox(player, settings, ticks / 100);
		SDL_RenderPresent(settings.renderer);
		ticks++;
	}

	close(settings, player);
	return 0;
}

#ifdef _WIN32
#define main SDL_main
#endif
