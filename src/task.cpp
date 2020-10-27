#include <task.h>

Task::Task() {
    this->_type = TK_NONE;
    this->_valid = false;
    this->_foodPower = 0;
    this->_decay = 0;
}

bool Task::valid() const {
    return this->_valid;
}

EnTask Task::type() const {
    return this->_type;
}

void Task::toOutputStream(std::ostream& os) const {
    os << this->_type;
}

void Task::fromInputStream(std::istream& is) {
    int c;
    is >> c;

    this->_valid = true;
    switch (c) {
        case TK_SURV_ST:
            this->_type = TK_SURV_ST;
            is >> this->_foodPower;
            is >> this->_decay;
            break;
        default:
            this->_type = TK_NONE;
            this->_valid = false;
    }
}

int Task::foodPower() const {
    return this->_foodPower;
}

int Task::decay() const {
    return this->_decay;
}

std::ostream& operator<<(std::ostream& os, const Task& task) {
    task.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Task& task) {
    task.fromInputStream(is);
    return is;
}
