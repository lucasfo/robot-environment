#ifndef __TILE_H
#define __TILE_H

#include <iostream>

enum EnTile {TL_NONE, TL_FREE, TL_WALL, TL_END};

enum EnTileFlags {
    TLF_AGENT = 0x01
    , TLF_FOOD = 0x02
    , TLF_VALID = 0x04
    , TLF_NOPRINT = 0x08
    , TLF_FOOTPRINT = 0x10
    , TLF_NEST = 0x20
    , TLF_DISCOVERED = 0x40
    , TLF_END = 0x80
};

enum EnTileColor { TLC_WHITE, TLC_GRAY, TLC_BLACK, TLC_DARK };

class Agent;

class Tile {
    private:
        EnTile _value;
        Agent* _agent;
        unsigned _flags;
        void valid (const bool valid);
        int  _color;
        int _pheromone;
        int _nestX;
        int _nestY;
    public:
        Tile();
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        EnTile value() const;
        bool valid() const;
        bool hasAgent() const;
        void getAgent(Agent* a);
        void loseAgent();
        bool hasFood() const;
        void getFood();
        void loseFood();
        bool hasNest() const;
        void getNest();
        void loseNest();
        bool hasFootPrint() const;
        void getFootPrint();
        void loseFootPrint();
        bool hasDiscovered() const;
        void getDiscovered();
        void loseDiscovered();
        int incPheromone(int inc);
        int color() const;
        void color(const int c);
        bool printable() const;
        void toPrint(bool value);
        Agent* agent() const;
        int getNestX() const;
        int getNestY() const;
        void setNestX(int x);
        void setNestY(int y);
};

std::ostream& operator<<(std::ostream& os, const Tile& c);
std::istream& operator>>(std::istream& is, Tile& tile);

#endif
