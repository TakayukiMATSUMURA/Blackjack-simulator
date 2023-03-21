/**
 * card.cc
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
#include "./card.h"


Card::Card(std::string arg) : 
        rank(arg[0] == 'A' ? 1 : arg[0] == 'T' ? 10 : arg[0] == 'J' ? 10 : arg[0] == 'Q' ? 10 : arg[0] == 'K' ? 10 : arg[0] - '0'),
        suit(arg[1] == 'c' ? 0 : arg[1] == 'd' ? 1 : arg[1] == 'h' ? 2 : arg[2] == 's' ? 3 : -1),
        _str(arg) {
}

Card* Card::getInstance(std::string str) {
    int rank = (str[0] == 'A') ? 1 :
        (str[0] == 'T') ? 10 :
        (str[0] == 'J') ? 11 :
        (str[0] == 'Q') ? 12 :
        (str[0] == 'K') ? 13 : str[0] - '0';
    char suit = (str[1] == 'c') ? 0 :
        (str[1] == 'd') ? 1 :
        (str[1] == 'h') ? 2 : 3;
    
    static Card allCards[52] = {
        Card("Ac"),
        Card("Ad"),
        Card("Ah"),
        Card("As"),
        Card("2c"),
        Card("2d"),
        Card("2h"),
        Card("2s"),
        Card("3c"),
        Card("3d"),
        Card("3h"),
        Card("3s"),
        Card("4c"),
        Card("4d"),
        Card("4h"),
        Card("4s"),
        Card("5c"),
        Card("5d"),
        Card("5h"),
        Card("5s"),
        Card("6c"),
        Card("6d"),
        Card("6h"),
        Card("6s"),
        Card("7c"),
        Card("7d"),
        Card("7h"),
        Card("7s"),
        Card("8c"),
        Card("8d"),
        Card("8h"),
        Card("8s"),
        Card("9c"),
        Card("9d"),
        Card("9h"),
        Card("9s"),
        Card("Tc"),
        Card("Td"),
        Card("Th"),
        Card("Ts"),
        Card("Jc"),
        Card("Jd"),
        Card("Jh"),
        Card("Js"),
        Card("Qc"),
        Card("Qd"),
        Card("Qh"),
        Card("Qs"),
        Card("Kc"),
        Card("Kd"),
        Card("Kh"),
        Card("Ks"),
    };
    
    int index = (rank - 1) * 4 + suit;
    return &allCards[index];
}

std::vector<Card*> Card::getDeck(int set) {
    std::vector<Card*> result;
    std::string ranks[13] = {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"
    };
    std::string suits[4] = {
        "c", "d", "h", "s"
    };

    for(int i = 0; i < set; i++) {
        for(int r = 0; r < 13; r++) {
            for(int s = 0; s <= 3; s++) {
                auto card = getInstance(ranks[r] + suits[s]);
                result.push_back(card);
            }
        }
    }
    return result;
}
