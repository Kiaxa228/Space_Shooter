/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-26-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: Animal.class.cpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#include "Animal.class.hpp"

Animal::Animal(int x) : AUnit()
{
    _type = 0;
    setX(x);
}

Animal::~Animal(void)
{
}

void Animal::moveDown(void)
{
    setY(getY() + 1);
}

void Animal::setType(int type)
{
    _type = type;
}

int Animal::getType(void)
{
    return _type;
}