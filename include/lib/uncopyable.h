#pragma once

template <class T>
class UnCopyable {
protected:
    UnCopyable() {}
    ~UnCopyable() {}

    UnCopyable(const T&) = delete;
    T& operator=(const T&) = delete;
};
