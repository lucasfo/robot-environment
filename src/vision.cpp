#include <vision.h>

Vision::Vision() {
    this->_range = 0;
    this->_vision = VI_NONE;
    this->_valid = false;
}

void Vision::toOutputStream(std::ostream& os) const {
    char c = '\0';
    switch (this->_vision) {
        case VI_BOX:
            c = '0';
            break;
        case VI_BFS:
            c = '1';
            break;
        case VI_BOX_BFS:
            c = '2';
            break;
        default:
            c = '?';
    }

    os << c << ' ' << this->_range;
}

bool Vision::valid() const {
    return this->_valid;
}

void Vision::fromInputStream(std::istream& is) {
    this->_valid = true;
    char c = '\0';
    is >> c;
    switch (c) {
        case '0':
            this->_vision = VI_BOX;
            break;
        case '1':
            this->_vision = VI_BFS;
            break;
        case '2':
            this->_vision = VI_BOX_BFS;
            break;
        default:
            this->_valid = false;
            this->_vision = VI_NONE;
            this->_range = 0;
            return;
    }

    is >> this->_range;
    if (this->_range <= 0) {
        this->_valid = false;
        this->_vision = VI_NONE;
        this->_range = 0;
        return;
    }
}

EnVision Vision::mode() const {
    return this->_vision;
}

int Vision::range() const {
    return this->_range;
}

std::ostream& operator<<(std::ostream& os, const Vision& vision) {
    vision.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Vision& vision) {
    vision.fromInputStream(is);
    return is;
}
