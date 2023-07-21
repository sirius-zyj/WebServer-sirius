#ifndef TYPE_H
#define TYPE_H

template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}

#endif