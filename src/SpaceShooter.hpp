/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-24-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: SpaceShooter.hpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#ifndef SPACESHOOTER_HPP
#define SPACESHOOTER_HPP

#include <iostream>
#include <string>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
#define INFOBOX_HEIGHT (WINDOW_HEIGHT / 5)

#define ENEMY_MAX_QUANTITY 10
#define ANIMAL_MAX_QUANTITY 10
#define MAX_BULLETS 100

#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50
#define PLAYER_WIDTH 33
#define PLAYER_HEIGHT 25
#define ANIMAL_WIDTH 50
#define ANIMAL_HEIGHT 50
#define FIREBALL_WIDTH 10
#define FIREBALL_HEIGHT 15

#define ANIMAL_HIT 20
#define ENEMY_HIT 50

#define ANIMAL_POINTS 30
#define DEER_POINTS 70
#define BEAR_POINTS 20
#define KANGAROO_POINTS 50
#define ENEMY_POINTS 75

#define PLAYER_START_X ((WINDOW_WIDTH / 2) - (PLAYER_WIDTH / 2))
#define PLAYER_START_Y (WINDOW_HEIGHT - PLAYER_HEIGHT - INFOBOX_HEIGHT + 30)

#define ENEMY_IMG ("img/enemy.png")
#define PLAYER_IMG ("img/player.png")
#define FIREBALL_IMG ("img/fireball.png")
#define GAME_OVER ("img/game_over.png")

#define FONT_TTF ("ttf/banksb20.ttf")

#define MUSIC ("audio/moons_001.wav")

class AUnit;
class Player;

typedef struct s_settings
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} t_settings;

#endif
