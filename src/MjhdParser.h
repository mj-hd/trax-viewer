#ifndef _MJHDPARSER_H_
#define _MJHDPARSER_H_

#include "IParser.h"

enum class MjhdParserState : int {
    Normal = 0,
    Column,
    Row
};

class MjhdParser : public IParser {
public:
    MjhdParser();
    ~MjhdParser();

    bool operator<<(std::istream& stream);

    const_iterator begin() const;
    const_iterator end() const;

protected:
    std::vector<std::vector<Tile>> _tiles;

private:
    MjhdParserState _state;

    unsigned int _width;
    unsigned int _index;
};

#endif
