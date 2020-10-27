#include <tile.h>

Tile::Tile() {
    this->_value = TL_NONE;
    this->_color = TLC_WHITE;
    this->_flags = 0;
    this->_agent = NULL;
    this->_pheromone = 0;
}

bool Tile::valid() const {
    return (this->_flags & TLF_VALID) > 0;
}

void Tile::valid (const bool valid) {
    if (valid) {
        this->_flags |= TLF_VALID;
    }

    else {
        this->_flags &= ~TLF_VALID;
    }
}

EnTile Tile::value() const {
    return this->_value;
}

void Tile::toOutputStream(std::ostream& os) const {
    if (this->printable()) {
        return;
    }

    if (!this->valid()) {
        os << '?';
    }

    else if (this->hasAgent()) {
        os << 'a';
    }

    else if (this->hasFood()) {
        os << '@';
    }

    else if (this->hasFootPrint()) {
        os << '-';
    }

    else if (this->hasNest()) {
        os << '*';
    }

    else if (this->_pheromone > 0) {
        os << '-';
    }

    else {
        switch (this->_value) {
            case TL_FREE:
                os << '.';
            break;
            case TL_WALL:
                os << '#';
            break;
            default:
                os << '?';
            break;
        }
    }

}

void Tile::fromInputStream(std::istream& is) {
    char c;
    is >> c;

    this->valid(true);
    switch (c) {
        case '.':
            this->_value = TL_FREE;
            break;
        case '@':
            this->_value = TL_FREE;
            this->getFood();
            break;
        case '#':
            this->_value = TL_WALL;
            break;
        case '*':
            this->_value = TL_FREE;
            this->getNest();
            break;
        default:
            this->_value = TL_NONE;
            this->valid(false);
            break;
    }
}

bool Tile::hasAgent() const {
    return (this->_flags & TLF_AGENT) > 0;
}

void Tile::getAgent(Agent* a) {
    this->_flags |= TLF_AGENT;
    this->_agent = a;
    return;
}

void Tile::loseAgent() {
    this->_flags &= ~TLF_AGENT;
    this->_agent = NULL;
    return;
}

bool Tile::hasFood() const {
    return (this->_flags & TLF_FOOD) > 0;
}

void Tile::getFood() {
    this->_flags |= TLF_FOOD;
    return;
}

void Tile::loseFood() {
    this->_flags &= ~TLF_FOOD;
    return;
}

bool Tile::hasNest() const {
    return (this->_flags & TLF_NEST) > 0;
}

void Tile::getNest() {
    this->_flags |= TLF_NEST;
    return;
}

void Tile::loseNest() {
    this->_flags &= ~TLF_NEST;
    return;
}

bool Tile::hasFootPrint() const {
    return (this->_flags & TLF_FOOTPRINT) > 0;
}

void Tile::getFootPrint() {
    this->_flags |= TLF_FOOTPRINT;
    return;
}

void Tile::loseFootPrint() {
    this->_flags &= ~TLF_FOOTPRINT;
    return;
}

bool Tile::hasDiscovered() const {
    return (this->_flags & TLF_DISCOVERED) > 0;
}

void Tile::getDiscovered() {
    this->_flags |= TLF_DISCOVERED;
    return;
}

void Tile::loseDiscovered() {
    this->_flags &= ~TLF_DISCOVERED;
    return;
}

int Tile::color() const {
    return this->_color;
}
void Tile::color(const int c) {
    this->_color = c;
}

bool Tile::printable() const {
    return (this->_flags & TLF_NOPRINT) > 0;
}

void Tile::toPrint(bool value) {
    if(!value) {
        this->_flags |= TLF_NOPRINT;
    }

    else {
        this->_flags &= ~TLF_NOPRINT;
    }
}

int Tile::incPheromone(int inc) {
    this->_pheromone += inc;
    if (this->_pheromone < 0) {
        this->_pheromone = 0;
    }

    return this->_pheromone;
}

Agent* Tile::agent() const {
    return this->_agent;
}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    tile.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Tile& tile) {
    tile.fromInputStream(is);
    return is;
}

int Tile::getNestX() const {
    return this->_nestX;
}

int Tile::getNestY() const {
    return this->_nestY;
}

void Tile::setNestX(int x) {
    this->_nestX = x;
}

void Tile::setNestY(int y) {
    this->_nestY = y;
}
