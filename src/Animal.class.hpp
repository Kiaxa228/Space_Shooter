/**
 * @Author: Anhelina Shulha <angieshu>
 * @Date:   Jul-26-2017
 * @Email:  anhelina.shulha@gmail.com
 * @Filename: Animal.class.hpp
 * @Last modified by:   angieshu
 * @Last modified time: Jul-26-2017
 */

#ifndef ANIMAL_CLASS_HPP
#define ANIMAL_CLASS_HPP

#include "SpaceShooter.hpp"
#include "AUnit.class.hpp"

class Animal : public AUnit
{
private:
    int _type; // 0: deer, 1: bear, 2: kangaroo

public:
    Animal(int x);
    ~Animal(void);

    void moveDown(void);
    void setType(int type);
    int getType(void);
};

#endif