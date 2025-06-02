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
	t_settings settings;
	Player player;
	Background background;
	SDL_Event e;
	bool quit = false;
	unsigned long tick = 0;

	if (!init(settings))
		return 1;

	while (!quit)
	{
		// Показываем главное меню при запуске
		bool inMainMenu = true;
		bool inHighScores = false;
		while (inMainMenu && !quit)
		{
			// Очищаем экран
			SDL_SetRenderDrawColor(settings.renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(settings.renderer);

			// Показываем главное меню или таблицу рекордов
			if (inHighScores)
			{
				background.showHighScores(settings);
			}
			else
			{
				background.showMainMenu(settings);
			}
			SDL_RenderPresent(settings.renderer);

			// Обрабатываем события
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					if (inHighScores)
					{
						// Check if Back button is clicked
						if (mouseX >= (WINDOW_WIDTH - 200) / 2 &&
							mouseX <= (WINDOW_WIDTH + 200) / 2 &&
							mouseY >= (WINDOW_HEIGHT - 50) / 2 + 250 &&
							mouseY <= (WINDOW_HEIGHT + 50) / 2 + 250)
						{
							inHighScores = false;
						}
					}
					else
					{
						// Check if Play button is clicked
						if (mouseX >= (WINDOW_WIDTH - 200) / 2 &&
							mouseX <= (WINDOW_WIDTH + 200) / 2 &&
							mouseY >= (WINDOW_HEIGHT - 50) / 2 - 40 &&
							mouseY <= (WINDOW_HEIGHT + 50) / 2 - 40)
						{
							inMainMenu = false;
							// Загружаем игровой фон
							if (!background.loadBackground("img/background.jpg", settings))
							{
								std::cout << "Failed to load background image. Returning to main menu." << std::endl;
								inMainMenu = true;
								continue;
							}
							// Загружаем изображение игрока
							if (!player.load(PLAYER_IMG, settings))
							{
								std::cout << "Failed to load Player image: " << PLAYER_IMG << std::endl;
								inMainMenu = true;
								continue;
							}
						}
						// Check if High Scores button is clicked
						else if (mouseX >= (WINDOW_WIDTH - 200) / 2 &&
								 mouseX <= (WINDOW_WIDTH + 200) / 2 &&
								 mouseY >= (WINDOW_HEIGHT - 50) / 2 + 40 &&
								 mouseY <= (WINDOW_HEIGHT + 50) / 2 + 40)
						{
							inHighScores = true;
						}
					}
				}
				else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
				{
					if (inHighScores)
					{
						inHighScores = false;
					}
					else
					{
						quit = true;
					}
				}
			}
		}

		// Основной игровой цикл
		while (!quit && !inMainMenu)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
					quit = true;
				else if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT:
					case SDLK_a:
						player.moveLeft();
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						player.moveRight();
						break;
					case SDLK_SPACE:
						player.shoot(settings);
						break;
					case SDLK_ESCAPE:
						quit = true;
						inMainMenu = true;
						break;
					default:
						break;
					}
				}
			}

			if (tick >= 90000)
			{
				// Game over
				bool scoreUpdated = false;
				bool gameOver = true;
				while (gameOver && !quit)
				{
					// Show game over screen with final score
					if (!scoreUpdated)
					{
						background.loadGameOver(settings, player.getPoints());
						scoreUpdated = true;
					}
					SDL_RenderPresent(settings.renderer);

					// Handle events in game over screen
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							quit = true;
							gameOver = false;
						}
						else if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							int mouseX, mouseY;
							SDL_GetMouseState(&mouseX, &mouseY);

							// Check if click is within Main Menu button area
							if (mouseX >= (WINDOW_WIDTH - 200) / 2 &&
								mouseX <= (WINDOW_WIDTH + 200) / 2 &&
								mouseY >= (WINDOW_HEIGHT - 50) / 2 + 50 &&
								mouseY <= (WINDOW_HEIGHT + 50) / 2 + 50)
							{
								// Return to main menu
								gameOver = false;
								inMainMenu = true;
								// Reset game state
								tick = 0;
								player.reset();
								background.clean();
								background.loadBackground("img/background.jpg", settings);
								gameOver = false;
							}
							// Check if click is within Play Again button area
							else if (mouseX >= (WINDOW_WIDTH - 200) / 2 &&
									 mouseX <= (WINDOW_WIDTH + 200) / 2 &&
									 mouseY >= (WINDOW_HEIGHT - 50) / 2 + 120 &&
									 mouseY <= (WINDOW_HEIGHT + 50) / 2 + 120)
							{
								// Reset game state
								tick = 0;
								player.reset();
								background.clean();
								background.loadBackground("img/background.jpg", settings);
								gameOver = false;
							}
						}
						else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
						{
							quit = true;
							gameOver = false;
						}
					}
				}
				continue;
			}

			if (tick % (ANIMAL_HEIGHT * 20) == 0)
				background.makeAnimal(settings);

			background.killAnimal(player);

			SDL_SetRenderDrawColor(settings.renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(settings.renderer);

			background.renderBackground(settings);
			background.displayAnimal(settings, (tick % 2 == 0));

			player.render(settings);
			player.moveBullets(settings);

			background.drawInfoBox(player, settings, tick);
			SDL_RenderPresent(settings.renderer);
			tick++;
		}
	}

	close(settings, player);
	return 0;
}

#ifdef _WIN32
#define main SDL_main
#endif
