/**
 * card.h
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

#include <vector>
#include <string>

enum {
    A = 1,
    T = 10,
};

class Card {
public:
    ~Card() {}

    const static std::string ranks[13];
    const static std::string suits[4];
    
    static Card* getInstance(std::string);
    
    static std::vector<Card*> getDeck(int = 1);
    
    std::string toString() const { return _str; }
    int rank() const { return _rank; }
    int suit() const {return _suit; }
    
private:
    Card(std::string);
    std::string _str;
    int _rank;
    int _suit;
};
