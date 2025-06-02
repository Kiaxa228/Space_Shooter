/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-25-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: Player.class.cpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#include "Player.class.hpp"
#include "Fireball.class.hpp"

int Player::_qBullets = 0;
long Player::_score = 0;

Player::Player(void)
{
	setWidth(PLAYER_WIDTH);
	setHeight(PLAYER_HEIGHT);
	setX(PLAYER_START_X);
	setY(PLAYER_START_Y);
	for (int i = 0; i < MAX_BULLETS; i++)
		_bullets[i] = NULL;
	_colliders.resize(2);
	_colliders[0].w = PLAYER_WIDTH / 3;
	_colliders[0].h = PLAYER_HEIGHT / 3;
	_colliders[1].w = (PLAYER_WIDTH / 3) * 2;
	_colliders[1].h = (PLAYER_HEIGHT / 3) * 2;
	shiftColliders();
}

Player::~Player(void)
{
}

int Player::getPoints(void)
{
	return _score;
}

void Player::setPoints(int points)
{
	_score += points;
}

void Player::moveDown(void)
{
	if (getY() >= WINDOW_HEIGHT - PLAYER_HEIGHT - 3)
		return;
	setY(getY() + 3);
}

void Player::shoot(t_settings &settings)
{
	if (_qBullets >= MAX_BULLETS)
		return;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (_bullets[i])
			continue;
		_bullets[i] = new Fireball(this);
		_bullets[i]->load(FIREBALL_IMG, settings);
		_qBullets++;
		break;
	}
}

void Player::moveBullets(t_settings &settings)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (_bullets[i])
		{
			if (_bullets[i]->getY() <= -FIREBALL_HEIGHT)
			{
				delete _bullets[i];
				_bullets[i] = NULL;
				_qBullets--;
				continue;
			}
			_bullets[i]->moveDown();
			_bullets[i]->render(settings);
		}
	}
}

void Player::shiftColliders(void)
{
	_colliders[0].x = getX() + (PLAYER_WIDTH / 3);
	_colliders[0].y = getY();
	_colliders[1].x = getX();
	_colliders[1].y = getY() + (PLAYER_HEIGHT / 3);
}

bool Player::containes(Enemy *enemy)
{
	shiftColliders();
	int topPlayer_0 = _colliders[0].y;
	int bottomPlayer_0 = _colliders[0].y + _colliders[0].h;
	int leftPlayer_0 = _colliders[0].x;
	int rightPlayer_0 = _colliders[0].x + _colliders[0].w;

	int topPlayer_1 = _colliders[1].y;
	int bottomPlayer_1 = _colliders[1].y + _colliders[1].h;
	int leftPlayer_1 = _colliders[1].x;
	int rightPlayer_1 = _colliders[1].x + _colliders[1].w;

	int topEnemy = enemy->getY();
	int bottomEnemy = enemy->getY() + ENEMY_HEIGHT;
	int leftEnemy = enemy->getX();
	int rightEnemy = enemy->getX() + ENEMY_WIDTH;

	if (((bottomPlayer_0 <= topEnemy) ||
		 (topPlayer_0 >= bottomEnemy) ||
		 (rightPlayer_0 <= leftEnemy) ||
		 (leftPlayer_0 >= rightEnemy)) == false)
		return true;

	if (((bottomPlayer_1 <= topEnemy) ||
		 (topPlayer_1 >= bottomEnemy) ||
		 (rightPlayer_1 <= leftEnemy) ||
		 (leftPlayer_1 >= rightEnemy)) == false)
		return true;

	return false;
}

bool Player::containes(Animal *animal)
{
	int topPlayer = getY();
	int bottomPlayer = getY() + PLAYER_HEIGHT;
	int leftPlayer = getX();
	int rightPlayer = getX() + PLAYER_WIDTH;

	int topAnimal = animal->getY();
	int bottomAnimal = animal->getY() + ANIMAL_HEIGHT;
	int leftAnimal = animal->getX();
	int rightAnimal = animal->getX() + ANIMAL_WIDTH;

	if (((bottomPlayer <= topAnimal) ||
		 (topPlayer >= bottomAnimal) ||
		 (rightPlayer <= leftAnimal) ||
		 (leftPlayer >= rightAnimal)) == false)
		return true;

	return false;
}

bool Player::kill(int index, Enemy *enemy)
{
	if (index >= MAX_BULLETS || !_bullets[index])
		return false;
	if (_bullets[index]->contains(enemy))
	{
		delete _bullets[index];
		_bullets[index] = NULL;
		return true;
	}
	return false;
}

bool Player::kill(int index, Animal *animal)
{
	if (index >= MAX_BULLETS || !_bullets[index] || !animal)
		return false;
	if (_bullets[index]->contains(animal))
	{
		delete _bullets[index];
		_bullets[index] = NULL;
		_qBullets--;
		_score += 10;
		return true;
	}
	return false;
}

bool Player::getKilled(int hit_points)
{
	return false; // Игрок больше не может быть убит
}

void Player::reset(void)
{
	// Reset player position
	setX(PLAYER_START_X);
	setY(PLAYER_START_Y);

	// Reset lives and score
	_score = 0;

	// Clear bullets
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (_bullets[i])
		{
			delete _bullets[i];
			_bullets[i] = NULL;
		}
	}
	_qBullets = 0;

	// Reset colliders
	_colliders.clear();
	_colliders.resize(2);
	_colliders[0].w = PLAYER_WIDTH / 3;
	_colliders[0].h = PLAYER_HEIGHT / 3;
	_colliders[1].w = (PLAYER_WIDTH / 3) * 2;
	_colliders[1].h = (PLAYER_HEIGHT / 3) * 2;
	shiftColliders();
}
