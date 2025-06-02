/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-25-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: Fireball.class.cpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#include "Fireball.class.hpp"
#include "Animal.class.hpp"
#include "Enemy.class.hpp"

Fireball::Fireball(Player *player)
{
	setWidth(FIREBALL_WIDTH);
	setHeight(FIREBALL_HEIGHT);
	setX(PLAYER_WIDTH / 3 + player->getX() + 1);
	setY(player->getY());
}

Fireball::~Fireball(void)
{
}

void Fireball::moveDown(void)
{
	setY(getY() - 1);
}

bool Fireball::contains(Enemy *enemy)
{
	int topBullet = this->getY();
	int bottomBullet = this->getY() + FIREBALL_HEIGHT;
	int leftBullet = this->getX();
	int rightBullet = this->getX() + FIREBALL_WIDTH;

	int topEnemy = enemy->getY();
	int bottomEnemy = enemy->getY() + ENEMY_HEIGHT;
	int leftEnemy = enemy->getX();
	int rightEnemy = enemy->getX() + ENEMY_WIDTH;

	if (((bottomBullet <= topEnemy) ||
		 (topBullet >= bottomEnemy) ||
		 (rightBullet <= leftEnemy) ||
		 (leftBullet >= rightEnemy)) == false)
		return true;

	return false;
}

bool Fireball::contains(Animal *animal)
{
	int topBullet = this->getY();
	int bottomBullet = this->getY() + FIREBALL_HEIGHT;
	int leftBullet = this->getX();
	int rightBullet = this->getX() + FIREBALL_WIDTH;

	int topAnimal = animal->getY();
	int bottomAnimal = animal->getY() + ANIMAL_HEIGHT;
	int leftAnimal = animal->getX();
	int rightAnimal = animal->getX() + ANIMAL_WIDTH;

	if (((bottomBullet <= topAnimal) ||
		 (topBullet >= bottomAnimal) ||
		 (rightBullet <= leftAnimal) ||
		 (leftBullet >= rightAnimal)) == false)
		return true;

	return false;
}
