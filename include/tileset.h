#ifndef __TILESET_H
#define __TILESET_H

#include <iostream>
#include <tile.h>
#include <sstream>

    class Tileset {
        private:
            int _width;
            int _height;
            bool _valid;
            bool _framed;
            bool _printAsLine;
            Tile* _tiles;
            void invalidate();
            void free();
            void cpy(const Tileset& other);
        public:
            int matrixToArray(const int i, const int j) const;
            const Tile& getTile(const int i, const int j) const;
            Tile& tile(const int i, const int j);
            Tileset();
            Tileset(const Tileset& other);
            ~Tileset();
            void toOutputStream(std::ostream& os) const;
            void fromInputStream(std::istream& is);
            bool valid() const;
            int width() const;
            int height() const;
            void resize(const int width, const int height);
            bool framed() const;
            void toFrame(bool b);
            bool printAsLine(bool b);
            void prepareToMove();
            void finishMoves(int inc);
            Tileset& operator=(const Tileset& other);
};

std::ostream& operator<<(std::ostream& os, const Tileset& tileset);
std::istream& operator>>(std::istream& is, Tileset& tileset);

#endif
