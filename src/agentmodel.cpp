#include <agentmodel.h>

AgentModel::AgentModel() {
    this->_valid = false;
    this->_maxPower = 0;
    this->_actionPower = 0;
    this->_footPrint = 0;
}

void AgentModel::toOutputStream(std::ostream& os) const {
    os << this->_vision << ' ';
    os << this->_maxPower << ' ';
    os << this->_actionPower << ' ' << this->_footPrint;
}

void AgentModel::fromInputStream(std::istream& is) {

    this->_valid = true;
    is >> this->_vision;
    is >> this->_maxPower;
    is >> this->_actionPower;
    is >> this->_footPrint;

    if (this->_vision.mode() == VI_NONE) {
        Printer::log()->warn("AgentModel vision mode with invalid value");
        this->_valid = false;
    }

    if (this->_vision.range() < 1) {
        Printer::log()->warn("AgentModel vision range with invalid value");
        this->_valid = false;
    }

    if (this->_maxPower < 1) {
        Printer::log()->warn("AgentModel max power with invalid value. (< 1).");
        this->_valid = false;
    }

    if (this->_actionPower < 0) {
        Printer::log()->warn("AgentModel action cost with invalid value. (< 0).");
        this->_valid = false;
    }

    if (this->_footPrint < 0) {
        Printer::log()->warn("AgentModel foor print with invalid value. (< 0).");
        this->_valid = false;
    }

    return;
}

bool AgentModel::valid() const {
    return this->_valid;
}

int AgentModel::maxPower() const {
    return this->_maxPower;
}

int AgentModel::actionPower() const {
    return this->_actionPower;
}

int AgentModel:: range() const {
    return this->_vision.range();
}

const Vision& AgentModel::vision() const {
    return this->_vision;
}

unsigned int AgentModel::footPrint() const {
    return this->_footPrint;
}

std::ostream& operator<<(std::ostream& os, const AgentModel& model) {
    model.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, AgentModel& model) {
    model.fromInputStream(is);
    return is;
}
