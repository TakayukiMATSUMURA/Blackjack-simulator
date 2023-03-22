/**
 * singleton.h
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

#include "./uncopyable.h"

template <class HostClass>
class Singleton : private UnCopyable<Singleton<HostClass>> {
public:
    static HostClass* instance() {
        if(_instance == nullptr)
            _instance = HostClass::create();

        return _instance;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    static HostClass* _instance;
};

template <class HostClass>
HostClass* Singleton<HostClass>::_instance = nullptr;
