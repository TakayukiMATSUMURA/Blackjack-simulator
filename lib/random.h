/**
 * random.h
 *
 * Copyright (C) 2017 Takayuki MATSUMURA
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See LICENSE file on the base directory for more information.
 *
 */
/**********************************************************************/
#pragma once

#include <random>

class Random {
public:
    int getValueFromUniform(int min, int max) {
        std::mt19937_64 mt(_rnd());
        std::uniform_int_distribution<int> rand(min, max);
        return rand(mt);
    }
    
    float getValueFromNormalDistribution(float mean, float sigma) {
        std::mt19937_64 mt(_rnd());
        std::normal_distribution<> dist(mean, sigma);
        return dist(mt);
    }
    
protected:

private:
    std::random_device _rnd;
};

class Dice {
public:
    Dice(int face = 6) : face(face), _random(new Random()) {
    }
    ~Dice() {
        delete _random;
    }
    
    int roll() {
        return _random->getValueFromUniform(1, face);
    }
    
    const int face;
    
private:
    Random* _random;
};
