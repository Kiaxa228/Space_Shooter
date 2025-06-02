/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-26-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: Background.class.hpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#ifndef BACKGROUND_CLASS_HPP
#define BACKGROUND_CLASS_HPP

#include "SpaceShooter.hpp"
#include "AUnit.class.hpp"
#include "Animal.class.hpp"
#include "Enemy.class.hpp"
#include "Player.class.hpp"
#include <vector>
#include <string>
#include <utility>

class Background : public AUnit
{
private:
	SDL_Texture *_texture;
	TTF_Font *_font;
	Enemy *_enemies[ENEMY_MAX_QUANTITY];
	Animal *_animals[ANIMAL_MAX_QUANTITY];
	int _initialX[ANIMAL_MAX_QUANTITY];
	int _initialY[ANIMAL_MAX_QUANTITY];
	std::vector<std::pair<std::string, int>> _highScores;
	Player *_player;

public:
	Background(void);
	~Background(void);

	void free(void);
	void clean(void);
	void loadHighScores(void);
	void saveHighScores(void);
	void updateHighScores(int score);
	void drawHighScores(t_settings &settings);
	void loadGameOver(t_settings &settings, int finalScore);
	void drawInfoBox(Player &player, t_settings &settings, unsigned long tick);
	void makeEnemy(t_settings &settings);
	void makeAnimal(t_settings &settings);
	void displayEnemy(t_settings &settings);
	void displayAnimal(t_settings &settings, bool itsTime);
	void hitEnemy(Player *player);
	bool hitAnimal(Player &player);
	void killEnemy(Player *player);
	void killAnimal(Player &player);
	bool loadBackground(std::string, t_settings &);
	void renderBackground(t_settings &);
	void showMainMenu(t_settings &settings);
	void showHighScores(t_settings &settings);
	void moveDown(void) override;
};

#endif
