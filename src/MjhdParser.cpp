#include "MjhdParser.h"

#include <iostream>
#include <string>
#include <boost/algorithm/string/predicate.hpp>

MjhdParser::MjhdParser() {
    this->_state = MjhdParserState::Normal;
}

MjhdParser::~MjhdParser() {
}

bool MjhdParser::operator<<(std::istream& stream) {
    std::string line;

    std::getline(stream, line);

    if (stream.eof()) throw "EOF";

    std::cout << line << std::endl;

    switch (this->_state) {
        case MjhdParserState::Normal:

            if (boost::starts_with(line, "  @")) {
                this->_width = (line.length() - 1) / 2;
                this->_index = 0;
                this->_tiles.clear();

                this->_state = MjhdParserState::Column;
            }

            break;
        case MjhdParserState::Column:

            if ('0' <= line[0] && line[0] <= '9') {

                bool wasWhiteSpace = false;
                this->_index++;
                this->_tiles.resize(this->_index + 1);

                for (auto it : line) {
                    Tile tile;

                    switch (it) {
                        case ' ':
                            if (!wasWhiteSpace) {
                                wasWhiteSpace = true;
                                continue;
                            } else {
                                tile = Tile {TileType::None, TileDirection::N};
                            }
                            break;
                        case 'a':
                            if (!wasWhiteSpace) throw "Unexpected Token 'a'";

                            tile = Tile {TileType::Cross, TileDirection::N};

                            break;
                        case 'b':
                            if (!wasWhiteSpace) throw "Unexpected Token 'b'";

                            tile = Tile {TileType::Cross, TileDirection::E};

                            break;
                        case 'c':
                            if (!wasWhiteSpace) throw "Unexpected Token 'c'";

                            tile = Tile {TileType::Slash, TileDirection::N};

                            break;
                        case 'd':
                            if (!wasWhiteSpace) throw "Unexpected Token 'd'";

                            tile = Tile {TileType::Slash, TileDirection::E};

                            break;
                        case 'e':
                            if (!wasWhiteSpace) throw "Unexpected Token 'e'";

                            tile = Tile {TileType::Slash, TileDirection::S};

                            break;
                        case 'f':
                            if (!wasWhiteSpace) throw "Unexpected Token 'f'";

                            tile = Tile {TileType::Slash, TileDirection::W};

                            break;
                    }

                    this->_tiles[this->_index].push_back(tile);

                    if (wasWhiteSpace) wasWhiteSpace = false;
                }
            } else {
                this->_state = MjhdParserState::Normal;
    
                return true;
            }

            break;
        case MjhdParserState::Row:
            break;
    }

    return false;
}

IParser::const_iterator MjhdParser::begin() const {
    return this->_tiles.begin();
}

IParser::const_iterator MjhdParser::end() const {
    return this->_tiles.end();
}
