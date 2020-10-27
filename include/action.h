#ifndef __ACTION_H
#define __ACTION_H

#include <iostream>

enum EnAction {
    AC_NONE
    , AC_UP
    , AC_RIGHT
    , AC_DOWN
    , AC_LEFT
    , AC_STAY
    , AC_ABORT
    , AC_FINISH
    , AC_END
};

class Action {
    private:
        EnAction _value;
        bool _valid;
    public:
        Action();
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        EnAction value() const;
        bool valid() const;
        EnAction set(char c);
};

std::ostream& operator<<(std::ostream& os, const Action& action);
std::istream& operator>>(std::istream& is, Action& action);

#endif
