#include <action.h>

Action::Action() {
    this->_value = AC_NONE;
    this->_valid = false;
}

bool Action::valid() const {
    return this->_valid;
}

EnAction Action::value() const {
    return this->_value;
}

void Action::toOutputStream(std::ostream& os) const {
    if (!this->_valid) {
        os << 'x';
    }
    else {
        switch (this->_value) {
            case AC_UP:
                os << 'u';
            break;
            case AC_RIGHT:
                os << 'r';
            break;
            case AC_DOWN:
                os << 'd';
            break;
            case AC_LEFT:
                os << 'l';
            break;
            case AC_ABORT:
                os << 'a';
            break;
            case AC_STAY:
                os << 's';
            break;
            case AC_FINISH:
                os << 'f';
            break;
            default:
                os << 'x';
            break;
        }
    }
}

void Action::fromInputStream(std::istream& is) {
    char c;
    if (is.good()) {
        is >> c;
    }

    else {
        c = '?';
    }

    this->set(c);

}

EnAction Action::set(char c) {
    this->_valid = true;
    switch (c) {
        case 'u':
            this->_value = AC_UP;
        break;
        case 'r':
            this->_value = AC_RIGHT;
        break;
        case 'd':
            this->_value = AC_DOWN;
        break;
        case 'l':
            this->_value = AC_LEFT;
        break;
        case 'a':
            this->_value = AC_ABORT;
        break;
        case 's':
            this->_value = AC_STAY;
        break;
        case 'f':
            this->_value = AC_FINISH;
        break;
        default:
            this->_value = AC_NONE;
            this->_valid = false;
        break;
    }

    return this ->_value;
}

std::ostream& operator<<(std::ostream& os, const Action& action) {
    action.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Action& action) {
    action.fromInputStream(is);
    return is;
}
