/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-26-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: Background.class.cpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#include "Background.class.hpp"
#include "Player.class.hpp"
#include <fstream>
#include <algorithm>
#include <ctime>

Background::Background(void)
{
	_texture = NULL;
	_font = NULL;
	for (int i = 0; i < ENEMY_MAX_QUANTITY; i++)
		_enemies[i] = NULL;
	for (int i = 0; i < ANIMAL_MAX_QUANTITY; i++)
	{
		this->_animals[i] = NULL;
		this->_initialX[i] = 0;
		this->_initialY[i] = 0;
	}

	// Load font with error checking
	_font = TTF_OpenFont(FONT_TTF, 20);
	if (!_font)
	{
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
	}

	loadHighScores();
}

Background::~Background(void)
{
	if (_font)
		TTF_CloseFont(_font);
	free();
	clean();
}

void Background::free(void)
{
	if (_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
		_texture = NULL;
	}
}

void Background::clean(void)
{
	free();
	for (int i = 0; i < ENEMY_MAX_QUANTITY; i++)
	{
		if (_enemies[i])
		{
			delete _enemies[i];
			_enemies[i] = NULL;
		}
	}
	for (int i = 0; i < ANIMAL_MAX_QUANTITY; i++)
	{
		if (this->_animals[i])
		{
			delete this->_animals[i];
			this->_animals[i] = NULL;
		}
	}
}

void Background::loadHighScores(void)
{
	_highScores.clear();
	std::ifstream file("highscores.txt");
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			size_t lastSpace = line.find_last_of(' ');
			if (lastSpace != std::string::npos)
			{
				std::string scoreStr = line.substr(lastSpace + 1);
				std::string date = line.substr(0, lastSpace);

				try
				{
					int score = std::stoi(scoreStr);
					_highScores.push_back({date, score});
				}
				catch (const std::exception &e)
				{
					// Silent error handling
				}
			}
		}
		file.close();
	}
}

void Background::saveHighScores(void)
{
	std::ofstream file("highscores.txt");
	if (file.is_open())
	{
		for (const auto &score : _highScores)
		{
			file << score.first << " " << score.second << std::endl;
		}
		file.close();
	}
}

void Background::updateHighScores(int score)
{
	// Get current time for the score entry
	time_t now = time(0);
	char *dt = ctime(&now);
	std::string timestamp(dt);
	timestamp = timestamp.substr(0, timestamp.length() - 1); // Remove newline

	// Add new score
	_highScores.push_back({timestamp, score});

	// Sort scores in descending order
	std::sort(_highScores.begin(), _highScores.end(),
			  [](const auto &a, const auto &b)
			  { return a.second > b.second; });

	// Keep only top 10 scores
	if (_highScores.size() > 10)
	{
		_highScores.resize(10);
	}

	// Save to file
	saveHighScores();
}

void Background::drawHighScores(t_settings &settings)
{
	TTF_Font *font = TTF_OpenFont(FONT_TTF, 20);
	SDL_Color fontColor = {0xFF, 0xFF, 0xFF}; // White color

	// Draw "High Scores" title
	std::string titleText = "High Scores";
	SDL_Surface *titleSurface = TTF_RenderText_Solid(font, titleText.c_str(), fontColor);
	SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(settings.renderer, titleSurface);

	SDL_Rect titleRect = {
		(WINDOW_WIDTH - titleSurface->w) / 2,
		(WINDOW_HEIGHT - titleSurface->h) / 2 - 150,
		titleSurface->w,
		titleSurface->h};

	SDL_RenderCopy(settings.renderer, titleTexture, NULL, &titleRect);
	SDL_FreeSurface(titleSurface);
	SDL_DestroyTexture(titleTexture);

	// Draw each high score
	int yOffset = titleRect.y + 40;
	for (size_t i = 0; i < _highScores.size(); ++i)
	{
		std::string scoreText = std::to_string(i + 1) + ". " +
								_highScores[i].first + " - " +
								std::to_string(_highScores[i].second);

		SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), fontColor);
		SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(settings.renderer, scoreSurface);

		SDL_Rect scoreRect = {
			(WINDOW_WIDTH - scoreSurface->w) / 2,
			yOffset,
			scoreSurface->w,
			scoreSurface->h};

		SDL_RenderCopy(settings.renderer, scoreTexture, NULL, &scoreRect);
		SDL_FreeSurface(scoreSurface);
		SDL_DestroyTexture(scoreTexture);

		yOffset += 30;
	}

	TTF_CloseFont(font);
}

void Background::loadGameOver(t_settings &settings, int finalScore)
{
	// Update high scores with the new score
	updateHighScores(finalScore);

	// Clear the screen with a dark background
	SDL_SetRenderDrawColor(settings.renderer, 0x1A, 0x1A, 0x1A, 0xFF); // Dark gray background
	SDL_RenderClear(settings.renderer);

	// Display final score
	TTF_Font *font = TTF_OpenFont(FONT_TTF, 30);
	SDL_Color fontColor = {0xE2, 0xE0, 0xE7};
	std::string scoreText = "Final Score: " + std::to_string(finalScore);
	SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), fontColor);
	SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(settings.renderer, scoreSurface);

	SDL_Rect scoreRect = {
		(WINDOW_WIDTH - scoreSurface->w) / 2,
		(WINDOW_HEIGHT - scoreSurface->h) / 2 - 50,
		scoreSurface->w,
		scoreSurface->h};

	SDL_RenderCopy(settings.renderer, scoreTexture, NULL, &scoreRect);

	// Create Main Menu button
	TTF_Font *buttonFont = TTF_OpenFont(FONT_TTF, 24);
	SDL_Color buttonColor = {0xFF, 0xFF, 0xFF};
	std::string mainMenuText = "Main Menu";
	SDL_Surface *mainMenuSurface = TTF_RenderText_Solid(buttonFont, mainMenuText.c_str(), buttonColor);
	SDL_Texture *mainMenuTexture = SDL_CreateTextureFromSurface(settings.renderer, mainMenuSurface);

	SDL_Rect mainMenuRect = {
		(WINDOW_WIDTH - mainMenuSurface->w) / 2,
		(WINDOW_HEIGHT - mainMenuSurface->h) / 2 + 50,
		mainMenuSurface->w,
		mainMenuSurface->h};

	SDL_SetRenderDrawColor(settings.renderer, 0x37, 0x30, 0x38, 0xFF);
	SDL_Rect mainMenuBgRect = {
		mainMenuRect.x - 20,
		mainMenuRect.y - 10,
		mainMenuRect.w + 40,
		mainMenuRect.h + 20};
	SDL_RenderFillRect(settings.renderer, &mainMenuBgRect);

	SDL_SetRenderDrawColor(settings.renderer, 0xF9, 0xF8, 0xAE, 0xFF);
	SDL_RenderDrawRect(settings.renderer, &mainMenuBgRect);

	SDL_RenderCopy(settings.renderer, mainMenuTexture, NULL, &mainMenuRect);

	// Create Play Again button
	std::string playAgainText = "Play Again";
	SDL_Surface *playAgainSurface = TTF_RenderText_Solid(buttonFont, playAgainText.c_str(), buttonColor);
	SDL_Texture *playAgainTexture = SDL_CreateTextureFromSurface(settings.renderer, playAgainSurface);

	SDL_Rect playAgainRect = {
		(WINDOW_WIDTH - playAgainSurface->w) / 2,
		(WINDOW_HEIGHT - playAgainSurface->h) / 2 + 120,
		playAgainSurface->w,
		playAgainSurface->h};

	SDL_SetRenderDrawColor(settings.renderer, 0x37, 0x30, 0x38, 0xFF);
	SDL_Rect playAgainBgRect = {
		playAgainRect.x - 20,
		playAgainRect.y - 10,
		playAgainRect.w + 40,
		playAgainRect.h + 20};
	SDL_RenderFillRect(settings.renderer, &playAgainBgRect);

	SDL_SetRenderDrawColor(settings.renderer, 0xF9, 0xF8, 0xAE, 0xFF);
	SDL_RenderDrawRect(settings.renderer, &playAgainBgRect);

	SDL_RenderCopy(settings.renderer, playAgainTexture, NULL, &playAgainRect);

	// Clean up
	SDL_FreeSurface(scoreSurface);
	SDL_DestroyTexture(scoreTexture);
	SDL_FreeSurface(mainMenuSurface);
	SDL_DestroyTexture(mainMenuTexture);
	SDL_FreeSurface(playAgainSurface);
	SDL_DestroyTexture(playAgainTexture);
	TTF_CloseFont(font);
	TTF_CloseFont(buttonFont);
}

void Background::drawInfoBox(Player &player, t_settings &settings, unsigned long tick)
{
	SDL_Rect infoBox = {0, WINDOW_HEIGHT, WINDOW_WIDTH, INFOBOX_HEIGHT};
	SDL_SetRenderDrawColor(settings.renderer, 0x37, 0x30, 0x38, 0xFF);
	SDL_RenderFillRect(settings.renderer, &infoBox);
	SDL_SetRenderDrawColor(settings.renderer, 0xF9, 0xF8, 0xAE, 0xFF);
	for (int i = 0; i < WINDOW_WIDTH; i += 4)
		SDL_RenderDrawPoint(settings.renderer, i, WINDOW_HEIGHT + 1);
	int pos_x = WINDOW_WIDTH / 25;
	int pos_y = WINDOW_HEIGHT + (INFOBOX_HEIGHT / 5);
	TTF_Font *font = TTF_OpenFont(FONT_TTF, 15);
	SDL_Color fontColor = {0xE2, 0xE0, 0xE7};

	std::string s = "SCORE:  " + std::to_string(player.getPoints());
	SDL_Surface *score = TTF_RenderText_Solid(font, s.c_str(), fontColor);
	SDL_Texture *messageScore = SDL_CreateTextureFromSurface(settings.renderer, score);
	SDL_Rect rect = {pos_x, pos_y, score->w, score->h};
	SDL_RenderCopy(settings.renderer, messageScore, NULL, &rect);
	SDL_FreeSurface(score);
	score = NULL;
	SDL_DestroyTexture(messageScore);
	messageScore = NULL;

	// Вычисляем оставшееся время (90 секунд - прошедшее время)
	int remainingTime = 90 - (tick / 1000); // tick в миллисекундах, делим на 1000 для получения секунд
	if (remainingTime < 0)
		remainingTime = 0; // Не показываем отрицательное время

	// Форматируем время в формат MM:SS
	int minutes = remainingTime / 60;
	int seconds = remainingTime % 60;
	char timeStr[6];
	snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);

	s = "TIME:    " + std::string(timeStr);
	SDL_Surface *ticks = TTF_RenderText_Solid(font, s.c_str(), fontColor);
	SDL_Texture *messageTicks = SDL_CreateTextureFromSurface(settings.renderer, ticks);
	rect = {pos_x, pos_y + (2 * INFOBOX_HEIGHT / 5), ticks->w, ticks->h};
	SDL_RenderCopy(settings.renderer, messageTicks, NULL, &rect);
	SDL_FreeSurface(ticks);
	ticks = NULL;
	SDL_DestroyTexture(messageTicks);
	messageTicks = NULL;

	TTF_CloseFont(font);
	font = NULL;
}

void Background::makeAnimal(t_settings &settings)
{
	static int lastSpawnTime = 0;
	int currentTime = SDL_GetTicks();

	if (currentTime - lastSpawnTime < 2000)
	{
		return;
	}
	lastSpawnTime = currentTime;

	for (int i = 0; i < ANIMAL_MAX_QUANTITY; i++)
	{
		if (this->_animals[i])
			continue;

		int pos_x = rand() % (WINDOW_WIDTH - ANIMAL_WIDTH);
		int maxY = PLAYER_START_Y - ANIMAL_HEIGHT;
		int pos_y = rand() % maxY;

		int animalType = rand() % 3;
		std::string animalImg;
		switch (animalType)
		{
		case 0:
			animalImg = "img/deer.png";
			break;
		case 1:
			animalImg = "img/bear.png";
			break;
		case 2:
			animalImg = "img/kangaroo.png";
			break;
		}

		this->_animals[i] = new Animal(pos_x);
		this->_animals[i]->setY(pos_y);
		this->_animals[i]->load(animalImg, settings);
		this->_animals[i]->setType(animalType);

		if (animalType == 1)
		{
			this->_initialX[i] = pos_x;
			this->_initialY[i] = pos_y;
		}
		break;
	}
}

void Background::displayAnimal(t_settings &settings, bool itsTime)
{
	static float angles[ANIMAL_MAX_QUANTITY] = {0.0f};
	static float radii[ANIMAL_MAX_QUANTITY] = {30.0f};
	static int directions[ANIMAL_MAX_QUANTITY] = {1};
	static int lastTime = SDL_GetTicks();
	static int lastChangeTime = SDL_GetTicks();
	static int lastDeerMoveTime = SDL_GetTicks();
	static int deerDirections[ANIMAL_MAX_QUANTITY] = {0};
	int currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	if (currentTime - lastChangeTime > 3000)
	{
		lastChangeTime = currentTime;
		for (int i = 0; i < ANIMAL_MAX_QUANTITY; i++)
		{
			Animal *currentAnimal = this->_animals[i];
			if (currentAnimal && currentAnimal->getType() == 1)
			{
				radii[i] = 20.0f + (rand() % 21);
				directions[i] = (rand() % 2) ? 1 : -1;
			}
		}
	}

	if (currentTime - lastDeerMoveTime > 2000)
	{
		lastDeerMoveTime = currentTime;
		for (int i = 0; i < ANIMAL_MAX_QUANTITY; i++)
		{
			Animal *currentAnimal = this->_animals[i];
			if (currentAnimal && currentAnimal->getType() == 0)
			{
				deerDirections[i] = rand() % 2;
			}
		}
	}

	for (int i = 0; i < ANIMAL_MAX_QUANTITY; i++)
	{
		Animal *currentAnimal = this->_animals[i];
		if (currentAnimal)
		{
			int animalType = currentAnimal->getType();
			int new_x = currentAnimal->getX();
			int new_y = currentAnimal->getY();

			if (animalType == 1)
			{
				float center_x = this->_initialX[i] + ANIMAL_WIDTH / 2;
				float center_y = this->_initialY[i] + ANIMAL_HEIGHT / 2;

				angles[i] = currentTime * 0.001f * directions[i];
				new_x = center_x + radii[i] * cos(angles[i]) - ANIMAL_WIDTH / 2;
				new_y = center_y + radii[i] * sin(angles[i]) - ANIMAL_HEIGHT / 2;
			}
			else if (animalType == 0)
			{
				int speed = 1;
				switch (deerDirections[i])
				{
				case 0:
					new_x += speed;
					break;
				case 1:
					new_x -= speed;
					break;
				}
			}
			else
			{
				int move_x = rand() % 3 - 1;
				int speed = 2;
				new_x = currentAnimal->getX() + move_x * speed;
			}

			if (new_x < 0)
				new_x = 0;
			if (new_x > WINDOW_WIDTH - ANIMAL_WIDTH)
				new_x = WINDOW_WIDTH - ANIMAL_WIDTH;

			int hunterLevel = PLAYER_START_Y - ANIMAL_HEIGHT;
			if (new_y > hunterLevel)
				new_y = hunterLevel;

			currentAnimal->setX(new_x);
			currentAnimal->setY(new_y);
			currentAnimal->render(settings);
		}
	}
}

bool Background::hitAnimal(Player &player)
{
	return false;
}

void Background::killAnimal(Player &player)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		for (int j = 0; j < ANIMAL_MAX_QUANTITY; j++)
		{
			if (this->_animals[j] && player.kill(i, this->_animals[j]))
			{
				int points = 0;
				switch (this->_animals[j]->getType())
				{
				case 0:
					points = DEER_POINTS;
					break;
				case 1:
					points = BEAR_POINTS;
					break;
				case 2:
					points = KANGAROO_POINTS;
					break;
				}
				delete this->_animals[j];
				this->_animals[j] = NULL;
				player.setPoints(points);
			}
		}
	}
}

bool Background::loadBackground(std::string path, t_settings &settings)
{
	free();

	// Check if file exists
	std::ifstream file(path.c_str());
	if (!file.good())
	{
		std::cout << "Background image file not found: " << path << std::endl;
		return false;
	}
	file.close();

	SDL_Texture *newTexture = NULL;
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		std::cout << "Failed to load background image: " << IMG_GetError() << std::endl;
		return false;
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x4F, 0x24, 0x7A));
	newTexture = SDL_CreateTextureFromSurface(settings.renderer, surface);
	if (newTexture == NULL)
	{
		std::cout << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return false;
	}

	SDL_FreeSurface(surface);
	_texture = newTexture;
	return true;
}

void Background::renderBackground(t_settings &settings)
{
	SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	SDL_RenderCopy(settings.renderer, _texture, NULL, &rect);
}

void Background::showMainMenu(t_settings &settings)
{
	// Загружаем фоновое изображение для главного меню
	free();
	SDL_Texture *newTexture = NULL;
	SDL_Surface *surface = IMG_Load("img/background.jpg");
	if (surface == NULL)
		std::cout << IMG_GetError() << std::endl;
	else
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x4F, 0x24, 0x7A));
		newTexture = SDL_CreateTextureFromSurface(settings.renderer, surface);
		if (newTexture == NULL)
			std::cout << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
	}
	_texture = newTexture;
	SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT + INFOBOX_HEIGHT};
	SDL_RenderCopy(settings.renderer, _texture, NULL, &rect);

	// Отображаем заголовок игры
	TTF_Font *titleFont = TTF_OpenFont(FONT_TTF, 50);
	SDL_Color titleColor = {0xFF, 0xFF, 0xFF}; // Белый цвет
	std::string titleText = "Space Shooter";
	SDL_Surface *titleSurface = TTF_RenderText_Solid(titleFont, titleText.c_str(), titleColor);
	SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(settings.renderer, titleSurface);

	// Центрируем заголовок
	SDL_Rect titleRect = {
		(WINDOW_WIDTH - titleSurface->w) / 2,
		(WINDOW_HEIGHT - titleSurface->h) / 4,
		titleSurface->w,
		titleSurface->h};

	SDL_RenderCopy(settings.renderer, titleTexture, NULL, &titleRect);

	// Создаем кнопки
	TTF_Font *buttonFont = TTF_OpenFont(FONT_TTF, 30);
	SDL_Color buttonColor = {0xFF, 0xFF, 0xFF}; // Белый цвет для текста кнопки

	// Play button
	std::string playText = "Play";
	SDL_Surface *playSurface = TTF_RenderText_Solid(buttonFont, playText.c_str(), buttonColor);
	SDL_Texture *playTexture = SDL_CreateTextureFromSurface(settings.renderer, playSurface);

	SDL_Rect playRect = {
		(WINDOW_WIDTH - playSurface->w) / 2,
		(WINDOW_HEIGHT - playSurface->h) / 2 - 40,
		playSurface->w,
		playSurface->h};

	SDL_SetRenderDrawColor(settings.renderer, 0x37, 0x30, 0x38, 0xFF);
	SDL_Rect playBgRect = {
		playRect.x - 20,
		playRect.y - 10,
		playRect.w + 40,
		playRect.h + 20};
	SDL_RenderFillRect(settings.renderer, &playBgRect);

	SDL_SetRenderDrawColor(settings.renderer, 0xF9, 0xF8, 0xAE, 0xFF);
	SDL_RenderDrawRect(settings.renderer, &playBgRect);

	SDL_RenderCopy(settings.renderer, playTexture, NULL, &playRect);

	// High Scores button
	std::string scoresText = "High Scores";
	SDL_Surface *scoresSurface = TTF_RenderText_Solid(buttonFont, scoresText.c_str(), buttonColor);
	SDL_Texture *scoresTexture = SDL_CreateTextureFromSurface(settings.renderer, scoresSurface);

	SDL_Rect scoresRect = {
		(WINDOW_WIDTH - scoresSurface->w) / 2,
		(WINDOW_HEIGHT - scoresSurface->h) / 2 + 40,
		scoresSurface->w,
		scoresSurface->h};

	SDL_SetRenderDrawColor(settings.renderer, 0x37, 0x30, 0x38, 0xFF);
	SDL_Rect scoresBgRect = {
		scoresRect.x - 20,
		scoresRect.y - 10,
		scoresRect.w + 40,
		scoresRect.h + 20};
	SDL_RenderFillRect(settings.renderer, &scoresBgRect);

	SDL_SetRenderDrawColor(settings.renderer, 0xF9, 0xF8, 0xAE, 0xFF);
	SDL_RenderDrawRect(settings.renderer, &scoresBgRect);

	SDL_RenderCopy(settings.renderer, scoresTexture, NULL, &scoresRect);

	// Очищаем ресурсы
	SDL_FreeSurface(titleSurface);
	SDL_DestroyTexture(titleTexture);
	SDL_FreeSurface(playSurface);
	SDL_DestroyTexture(playTexture);
	SDL_FreeSurface(scoresSurface);
	SDL_DestroyTexture(scoresTexture);
	TTF_CloseFont(titleFont);
	TTF_CloseFont(buttonFont);
}

void Background::showHighScores(t_settings &settings)
{
	// Load high scores from file
	loadHighScores();

	// Clear screen with dark background
	SDL_SetRenderDrawColor(settings.renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(settings.renderer);

	// Initialize font if not already initialized
	if (!_font)
	{
		_font = TTF_OpenFont(FONT_TTF, 20);
		if (!_font)
		{
			return;
		}
	}

	// Render title
	SDL_Surface *titleSurface = TTF_RenderText_Solid(_font, "High Scores", {255, 255, 255, 255});
	SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(settings.renderer, titleSurface);
	SDL_Rect titleRect = {(WINDOW_WIDTH - titleSurface->w) / 2, (WINDOW_HEIGHT - titleSurface->h) / 4 - 100, titleSurface->w, titleSurface->h};
	SDL_RenderCopy(settings.renderer, titleTexture, NULL, &titleRect);
	SDL_FreeSurface(titleSurface);
	SDL_DestroyTexture(titleTexture);

	// Render high scores
	int y = (WINDOW_HEIGHT - titleSurface->h) / 4 - 50;
	for (size_t i = 0; i < _highScores.size(); ++i)
	{
		std::string scoreText = std::to_string(i + 1) + ". " + _highScores[i].first + " - " + std::to_string(_highScores[i].second);
		SDL_Surface *scoreSurface = TTF_RenderText_Solid(_font, scoreText.c_str(), {255, 255, 255, 255});
		SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(settings.renderer, scoreSurface);
		SDL_Rect scoreRect = {(WINDOW_WIDTH - scoreSurface->w) / 2, y, scoreSurface->w, scoreSurface->h};
		SDL_RenderCopy(settings.renderer, scoreTexture, NULL, &scoreRect);
		SDL_FreeSurface(scoreSurface);
		SDL_DestroyTexture(scoreTexture);
		y += 40;
	}

	// Render Back button with background
	SDL_Surface *backSurface = TTF_RenderText_Solid(_font, "Back to Menu", {255, 255, 255, 255});
	SDL_Texture *backTexture = SDL_CreateTextureFromSurface(settings.renderer, backSurface);
	SDL_Rect backRect = {(WINDOW_WIDTH - backSurface->w) / 2, (WINDOW_HEIGHT - backSurface->h) / 2 + 250, backSurface->w, backSurface->h};

	// Draw button background
	SDL_SetRenderDrawColor(settings.renderer, 0x37, 0x30, 0x38, 0xFF);
	SDL_Rect backBgRect = {
		backRect.x - 20,
		backRect.y - 10,
		backRect.w + 40,
		backRect.h + 20};
	SDL_RenderFillRect(settings.renderer, &backBgRect);

	SDL_SetRenderDrawColor(settings.renderer, 0xF9, 0xF8, 0xAE, 0xFF);
	SDL_RenderDrawRect(settings.renderer, &backBgRect);

	SDL_RenderCopy(settings.renderer, backTexture, NULL, &backRect);
	SDL_FreeSurface(backSurface);
	SDL_DestroyTexture(backTexture);
}

void Background::moveDown(void)
{
	// Background doesn't need to move down
	// This is just a stub implementation to satisfy the abstract class requirement
}
