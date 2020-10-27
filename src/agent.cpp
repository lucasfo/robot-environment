#include <agent.h>

Agent::Agent(const AgentModel& m, int i, int j) {
    this->_i = i;
    this->_j = j;
    this->_lastI = i;
    this->_lastJ = j;
    this->_power = m.maxPower();
    this->_valid = false;
    this->_model = m;
    this->_toUndo = false;
    this->_hasFood = false;

    const int& range = this->_model.vision().range();
    const EnVision& vision = this->_model.vision().mode();

    const int length = 2*range +1;
    this->_perception.resize(length, length);
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            Tile& t = this->_perception.tile(i,j);
            t.color(TLC_DARK);
        }
    }
    switch(vision) {
        case VI_BOX: {
            for (int i = 0; i < length; ++i) {
                for (int j = 0; j < length; ++j) {
                    Tile& t = this->_perception.tile(i,j);
                    t.color(TLC_WHITE);
                }
            }
            break;
        }
        case VI_BOX_BFS:
        case VI_BFS: {
            for (int i = 0; i < length; ++i) {
                for (int j = 0; j < length; ++j) {
                    Tile& t = this->_perception.tile(i,j);
                    t.color(TLC_DARK);
                    if(vision == VI_BFS) {
                        t.toPrint(false);
                    }
                }
            }
            for (int i = 0; i < length; ++i) {
                int jMin=(i < range) ? range - i : i - range;
                int jMax=(i < range) ? range + i + 1 : 3*range +1 -i;
                for (int j = jMin; j < jMax; ++j) {
                    Tile& t = this->_perception.tile(i,j);
                    t.color(TLC_WHITE);
                }

            }

            break;
        }
        default:
            return;
    }

    if (this->_i < 0) {
        Printer::log()->warn("Agent 'i' position with invalid value. (< 0)");
        this->_valid = false;
    }

    if (this->_j < 0) {
        Printer::log()->warn("Agent 'j' position with invalid value. (< 0).");
        this->_valid = false;
    }
}

void Agent::toOutputStream(std::ostream& os) const {
    os << this->_i << ' ' << this->_j << ' ';
    os << this->_power;
}

void Agent::fromInputStream(std::istream& is) {

    this->_valid = true;
    is >> this->_i;
    is >> this->_j;


    return;
}

bool Agent::valid() const {
    return this->_valid;
}

int Agent::i() const {
    return this->_i;
}

int Agent::j() const {
    return this->_j;
}

void Agent::incrementPosition(const int iInc, const int jInc) {
    this->_lastI = this->_i;
    this->_lastJ = this->_j;
    this->_i += iInc;
    this->_j += jInc;
}

void Agent::undoIncrement() {
    this->_i = this->_lastI;
    this->_j = this->_lastJ;
}

int Agent::power() const {
    return this->_power;
}

int Agent::powerUp(const int inc) {
    this->_power += inc;
    if (this->_power > this->_model.maxPower()) {
        this->_power = this->_model.maxPower();
    }
    return this->_power;
}
int Agent::doAction() {
    this->_power -= this->_model.actionPower();
    return this->_power;
}

bool Agent::alive() const {
    return this->_power > 0;
}

int Agent:: range() const {
    return this->_model.vision().range();
}

Tileset& Agent::perception() {
    return this->_perception;
}

const AgentModel& Agent::model() const {
    return this->_model;
}

int Agent::vision() const {
    return this->_model.vision().mode();
}

unsigned int Agent::footPrint() const {
    return this->_model.footPrint();
}

bool Agent::setToUndo(bool toUndo) {
    if (!toUndo) {
        return this->_toUndo = toUndo;
    }

    else if (this->_lockUndo) {
        return this->_toUndo;
    }

    else {
        this->_lockUndo = true;
        return this->_toUndo = toUndo;
    }
}

void Agent::freeUndo() {
    this->_lockUndo = false;
}

bool Agent::hasToUndo() const {
    return this->_toUndo;
}

bool Agent::hasFood() const {
    return this->_hasFood;
}

void Agent::getFood() {
    this->_hasFood = true;
    return;
}

void Agent::loseFood() {
    this->_hasFood = false;
    return;
}

std::ostream& operator<<(std::ostream& os, const Agent& agent) {
    agent.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Agent& agent) {
    agent.fromInputStream(is);
    return is;
}
