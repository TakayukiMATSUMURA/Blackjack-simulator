/**
 * Factory.h
 *
 * Copyright (C) 2015 Takayuki MATSUMURA
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

#include <map>
#include <string>
#include <exception>

template<
    class AbstractProduct,
    class IdentifierType,
    class ProductCreator = std::function<AbstractProduct*()>
    >
class Factory {
public:
    /// Destructor
    virtual ~Factory() {};
    
    bool add(const IdentifierType& id, ProductCreator creator) {
        _associations[id] = creator;
        return true;
    }

    bool unregister(const IdentifierType& id) {
        return _associations.erase(id) == 1;
    }
    
    template<class... ArgTypes>
    AbstractProduct* createFrom(const IdentifierType& id,
                                const ArgTypes&... arg) const {
        try {
            auto it = _associations.find(id);
            if(it == _associations.end())
                throw std::invalid_argument("Factory invalid id: ");
            
            return it->second(arg...);
        }
        catch (std::exception& e) {
            std::cerr << e.what() << id  << std::endl;
        }
        return nullptr;
    }

private:
    std::map<IdentifierType, ProductCreator> _associations;
};
