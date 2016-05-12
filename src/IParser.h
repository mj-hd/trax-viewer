#ifndef _IPARSER_H_
#define _IPARSER_H_

#include <iostream>
#include <vector>

#include "Tile.h"

class IParser {
public:
    virtual ~IParser() {};

    virtual bool operator<<(std::istream& stream) = 0;

    typedef std::vector<std::vector<Tile>>::const_iterator const_iterator;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
};

#endif
