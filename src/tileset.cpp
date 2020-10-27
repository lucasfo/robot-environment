#include <tileset.h>

Tileset::Tileset() {
    this->invalidate();
    this->_tiles = NULL;
    this->_printAsLine = false;
}

Tileset::Tileset(const Tileset& other) {
    this->_tiles = NULL;
    this->_width = other._width;
    this->_height = other._height;
    this->_valid = other._valid;
    this->_printAsLine = other._printAsLine;
    if(other._tiles == NULL) {
        this->_tiles = NULL;
    }

    else {

        this->_tiles = new Tile[this->_width*this->_height];
        for(int i  = 0; i < this->_width*this->_height; ++i) {
            this->_tiles[i] = other._tiles[i];
        }
    }
}

Tileset::~Tileset() {
    delete[] this->_tiles;
    this->_tiles = NULL;
}

int Tileset::matrixToArray(int i, int j) const {
    return i*this->_width + j;
}

void Tileset::toOutputStream(std::ostream& os) const {
    for (int i = 0; i < this->_height; ++i) {
        for (int j = 0; j < this->_width; ++j) {
            os << this->_tiles[this->matrixToArray(i,j)];
        }

        // remove trailing space
        if (!this->_printAsLine) {
            os << std::endl;
        }
    }

    if (this->_printAsLine) {
        os << std::endl;
    }
}

void Tileset::invalidate() {
    this->_valid = false;
    this->_width = 0;
    this->_height = 0;
}

void Tileset::free() {
    delete[] this->_tiles;
    this->_tiles = NULL;
}

bool Tileset::valid() const {
    return this->_valid;
}

void Tileset::fromInputStream(std::istream& is) {
    is >> this->_width >> this->_height;

    int iStart = 0;
    int jStart = 0;
    int width = this->_width;
    int height =this->_height;

    if(this->_framed) {
        iStart = jStart = 1;
        this->_width +=2;
        this->_height +=2;
        ++width;
        ++height;
    }

    this->resize(this->_width, this->_height);

    for (int i = iStart; i < height; ++i) {
        for (int j = jStart; j < width; ++j) {
            Tile& tile = this->_tiles[this->matrixToArray(i,j)];
            is >> tile;
            if (!tile.valid()) {
                this->invalidate();
                this->free();
                return;
            }

        }
    }

    if(this->_framed) {
        for (int i = 0; i < this->_height; ++i) {
            Tile& tile1 = this->_tiles[this->matrixToArray(i,0)];
            Tile& tile2 = this->_tiles[this->matrixToArray(i,this->_width-1)];
            std::stringstream ss;
            ss << "##";
            ss >> tile1 >> tile2;
        }

        for (int i = 0; i < this->_width; ++i) {
            Tile& tile1 = this->_tiles[this->matrixToArray(0, i)];
            Tile& tile2 = this->_tiles[this->matrixToArray(this->_height-1, i)];
            std::stringstream ss;
            ss << "##";
            ss >> tile1 >> tile2;
        }
    }
}

void Tileset::resize(const int width, const int height) {
    this->free();
    this->_valid = true;
    this->_width = width;
    this->_height = height;

    if (this->_width < 1 || this->_height < 1) {
        this->invalidate();
        return;
    }

    this->_tiles = new Tile[this->_width*this->_height];
}


const Tile& Tileset::getTile(const int i, const int j) const {
    Tile nullTile;
    int index = this->matrixToArray(i,j);
    return (index > -1) ? this->_tiles[index] : nullTile;
}

Tile& Tileset::tile(const int i, const int j) {
    Tile nullTile;
    int index = this->matrixToArray(i,j);
    return (index > -1) ? this->_tiles[index] : nullTile;
}

int Tileset::width() const {
    return this->_width;
}

int Tileset::height() const {
    return this->_height;
}


void Tileset::cpy(const Tileset& other) {
    delete[] this->_tiles;
    this->_tiles = NULL;
    this->_width = other._width;
    this->_height = other._height;
    this->_valid = other._valid;
    this->_printAsLine = other._printAsLine;
    if(other._tiles == NULL) {
        this->_tiles = NULL;
    }

    else {

        this->_tiles = new Tile[this->_width*this->_height];
        for(int i  = 0; i < this->_width*this->_height; ++i) {
            this->_tiles[i] = other._tiles[i];
        }
    }
}

bool Tileset::framed() const {
    return this->_framed;
}

void Tileset::toFrame(bool b) {
    this->_framed = b;
}

bool Tileset::printAsLine(bool b) {
    this->_printAsLine = b;
    return this->_printAsLine;
}

void Tileset::prepareToMove() {
    for(int i  = 0; i < this->_width*this->_height; ++i) {
        this->_tiles[i].loseAgent();
    }
}

void Tileset::finishMoves(int inc) {
    for(int i  = 0; i < this->_width*this->_height; ++i) {
        this->_tiles[i].loseAgent();
        this->_tiles[i].incPheromone(-inc);
    }
}

Tileset& Tileset::operator=(const Tileset& other) {
    this->cpy(other);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Tileset& tileset) {
    tileset.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Tileset& tileset) {
    tileset.fromInputStream(is);
    return is;
}
