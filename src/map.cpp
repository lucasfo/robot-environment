#include <map.h>

Map::Map() {
    this->_tileset.toFrame(true);
    this->_printAsLine = false;
}

void Map::toOutputStream(std::ostream& os) const {
    os << this->_tileset;
}

bool Map::valid() const {
    return this->_tileset.valid();
}

void Map::fromInputStream(std::istream& is) {
    is >> this->_tileset;
    this->setNestVector();
}

void Map::moveAgent(Agent& agent) {
    Tile& target = this->_tileset.tile(agent.i(), agent.j());
    if (target.hasAgent()){ // Agent Colision
        agent.setToUndo(true);
        target.agent()->setToUndo(true);
        Stats::stats()->event(EV_COLIDE);// This is not 100% accurate
    }

    else {
        target.getAgent(&agent);
    }
}

bool Map::perform(const std::list<Action>& actions, const Task& task) {
    int iInc = 0;
    int jInc = 0;
    bool alive = false;
    this->_tileset.prepareToMove();
    std::list<Agent>::iterator agentsIt = this->_agents.begin();
    std::list<Action>::const_iterator actionsIt = actions.begin();
    for(; actionsIt != actions.end(); ++actionsIt, ++agentsIt) {
        const Action& action = *actionsIt;
        switch(action.value()) {
            case AC_UP:
                iInc = -1;
                jInc = 0;
                break;
            case AC_RIGHT:
                iInc = 0;
                jInc = 1;
                break;
            case AC_DOWN:
                iInc = 1;
                jInc = 0;
                break;
            case AC_LEFT:
                iInc = 0;
                jInc = -1;
                break;
            case AC_STAY:
                iInc = 0;
                jInc = 0;
                break;
            case AC_ABORT:
                return false;
            case AC_FINISH:
                return false;
            default:
                continue;
                break;

        }
        Agent& agent = *agentsIt;
        if (!agent.alive()) {
            continue;
        }
        const Tile& t = this->getTile(agent, iInc, jInc);
        if (t.hasDiscovered()) {
            Stats::stats()->event(EV_EXPLOIT);
        }
        else {
            Stats::stats()->event(EV_EXPLORE);
            this->_tileset.tile(agent.i() + iInc, agent.j() + jInc).getDiscovered();
        }
        agent.incrementPosition(iInc, jInc);
        switch (t.value()) {
            case TL_FREE:
                this->moveAgent(agent);
                agent.doAction();
                if (t.hasFood() && !agent.hasFood()) {
                    this->feedAgent(task, agent);
                }
                break;
            case TL_WALL:
                agent.doAction();
                agent.undoIncrement();
                this->moveAgent(agent);
                Stats::stats()->event(EV_COLIDE);
            default:
                break;
        }
        Stats::stats()->event(EV_ACT);
        if (agent.hasFood()) {
            Stats::stats()->event(EV_ACT_LOAD);
        }

        else {
            Stats::stats()->event(EV_ACT_UNLOAD);
        }
        alive |= agent.alive();
    }

    bool hasToUndo = true;
    while(hasToUndo) {
        hasToUndo = false;
        agentsIt = this->_agents.begin();
        for(; agentsIt != this->_agents.end(); ++agentsIt) {
            Agent& agent = *agentsIt;
            if (agent.hasToUndo()) {
                agent.undoIncrement();
                this->moveAgent(agent);
                agent.setToUndo(false);
                hasToUndo = true;
            }
        }
    }

    this->_tileset.finishMoves(task.decay());
    agentsIt = this->_agents.begin();
    for(; agentsIt != this->_agents.end(); ++agentsIt) {
        Agent& agent = *agentsIt;
        agent.freeUndo();
        Tile& t = this->_tileset.tile(agent.i(), agent.j());
        t.incPheromone(agent.footPrint());
        t.getAgent(&agent);
        if (t.hasNest()) {
            agent.powerUp(task.foodPower());
            if (agent.hasFood()) {
                agent.loseFood();
                Stats::stats()->event(EV_FEED);
            }
        }
    }
    return alive;
}

void Map::feedAgent(const Task& task, Agent& a) {
    Tile& t = this->_tileset.tile(a.i(), a.j());
    t.loseFood();
    Stats::stats()->event(EV_LOAD);
    a.getFood();
}

const Tile& Map::getTile(const int i, const int j) const {
    return this->_tileset.getTile(i,j);
}
const Tile& Map::getTile(const Agent& a, const int iInc, const int jInc) const {
    return this->_tileset.getTile(a.i() + iInc, a.j() + jInc);
}

const Tile& Map::getTile(const Agent& agent) const {
    return this->_tileset.getTile(agent.i(), agent.j());
}

void Map::percept() {
    std::ostream& os = Printer::print()->stream();
    for(auto it = this->_agents.begin(); it != this->_agents.end(); ++it) {
        Agent& agent = *it;
        os << agent.power();
        os << ' ';
        os << ((agent.hasFood())? 1 : 0);
        if (this->_printAsLine) {
            os << ' ';
        }
        else {
            os << std::endl;
        }
        Tileset& perception = agent.perception();
        int size = 2*agent.range() +1;
        bool init = true;
        std::queue<int> queueI;
        std::queue<int> queueJ;
        queueI.push(agent.i());
        queueJ.push(agent.j());
        Tile empty;
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                if (perception.tile(i, j).color() != TLC_DARK) {
                    perception.tile(i, j) = empty;
                    perception.tile(i, j).color(TLC_WHITE);
                }
            }
        }
        while(queueI.size() > 0 && queueJ.size() > 0) {
            const int i = queueI.front();
            queueI.pop();
            const int j = queueJ.front();
            queueJ.pop();
            const Tile& t = this->getTile(i,j);
            const int pi = i - agent.i() + perception.height()/2;
            const int pj = j - agent.j() + perception.width()/2;
            Tile& pt = perception.tile(pi, pj);
            pt = t;
            pt.color(TLC_BLACK);
            if (init || pt.value() != TL_WALL) {
                if (pi + 1 < size && perception.tile(pi +1, pj).color() == TLC_WHITE) {
                    queueI.push(i+1);
                    queueJ.push(j);
                    perception.tile(pi +1, pj).color(TLC_GRAY);
                }

                if (pi - 1 > -1 && perception.tile(pi -1, pj).color() == TLC_WHITE) {
                    queueI.push(i-1);
                    queueJ.push(j);
                    perception.tile(pi -1, pj).color(TLC_GRAY);
                }

                if (pj + 1 < size && perception.tile(pi, pj+1).color() == TLC_WHITE) {
                    queueI.push(i);
                    queueJ.push(j +1);
                    perception.tile(pi, pj+1).color(TLC_GRAY);
                }

                if (pj -1 > -1 && perception.tile(pi, pj-1).color() == TLC_WHITE) {
                    queueI.push(i);
                    queueJ.push(j -1);
                    perception.tile(pi, pj-1).color(TLC_GRAY);
                }
            }
            init = false;
        }

        //os << perception; // Normal perception
        this->compactPerception(os, perception); // Compacted perception
    }
}

bool Map::updateAgent(AgentModel& model, std::list<int> agentI, std::list<int> agentJ, bool printAsLine) {
    std::list<int>::iterator i = agentI.begin();
    std::list<int>::iterator j = agentJ.begin();
    this->_agents.clear();
    this->_printAsLine = printAsLine;
    for(;i != agentI.end() && j != agentJ.end(); ++i, ++j) {
        if (*i+1 < this->_tileset.height() && *j+1 < this->_tileset.width()) {
            this->_agents.push_back(Agent(model, *i, *j));
            Agent& agent = this->_agents.back();
            agent.perception().printAsLine(printAsLine);
            agent.incrementPosition(1,1); // For the frame
            Tile& t = this->_tileset.tile(agent.i(), agent.j());
            if (t.value() == TL_FREE) {
                // Need to uncheck if goes wrong
                t.getAgent(&agent);
            }

            else {
                return false;
            }
        }

        else {
            return false;
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const Map& map) {
    map.toOutputStream(os);
    return os;
}

std::istream& operator>>(std::istream& is, Map& map) {
    map.fromInputStream(is);
    return is;
}

void Map::compactPerception(std::ostream& os, const Tileset& s) {
    //os << s;

    //            0  A  V  >  <
    int acc[5] = {0, 0, 0, 0, 0};
    int x  [5] = {0, 0, 0, 1, -1};
    int y  [5] = {0, -1, 1, 0, 0};

    const int ai = s.height()/2;
    const int aj = s.width()/2;

    for(int i = 0; i < s.height(); ++i) {
        for(int j = 0; j < s.width(); ++j) {
            const int pi = i - ai;
            const int pj = j - aj;

            if (s.getTile(i, j).hasFood()) {
                if (pi < 0) {
                    if (pj < pi) {
                        acc[4] += (ai + pi + 1) + (aj + pj + 1);
                    }

                    else if (pj == pi) {
                        acc[4] += (ai + pi + 1) + (aj + pj + 1);
                        acc[1] += (ai + pi + 1) + (aj + pj + 1);
                    }

                    else if (pj < 0) {
                        acc[1] += (ai + pi + 1) + (aj + pj + 1);
                    }

                    else if (pj == 0) {
                        acc[1] += (ai + pi + 1) + (aj + pj + 1);
                    }

                    else if (pj < -pi) {
                        acc[1] += (ai + pi + 1) + (aj - pj + 1);
                    }

                    else if (pj == -pi) {
                        acc[1] += (ai + pi + 1) + (aj - pj + 1);
                        acc[3] += (ai + pi + 1) + (aj - pj + 1);
                    }

                    else {
                        acc[3] += (ai + pi + 1) + (aj - pj + 1);
                    }
                }

                else if (pi == 0) {
                    if (pj < 0) {
                        acc[4] += (ai + pi + 1) + (aj + pj + 1);
                    }

                    if (pj > 0) {
                        acc[3] += (ai + pi + 1) + (aj + pj + 1);
                    }
                }

                else {
                    if (pj < -pi) {
                        acc[4] += (ai - pi + 1) + (aj + pj + 1);
                    }

                    else if (pj == -pi) {
                        acc[4] += (ai - pi + 1) + (aj + pj + 1);
                        acc[2] += (ai - pi + 1) + (aj + pj + 1);
                    }

                    else if (pj < 0) {
                        acc[2] += (ai - pi + 1) + (aj + pj + 1);
                    }

                    else if (pj == 0) {
                        acc[2] += (ai - pi + 1) + (aj + pj + 1);
                    }

                    else if (pj < pi) {
                        acc[2] += (ai - pi + 1) + (aj - pj + 1);
                    }

                    else if (pj == pi) {
                        acc[2] += (ai - pi + 1) + (aj - pj + 1);
                        acc[3] += (ai - pi + 1) + (aj - pj + 1);
                    }

                    else {
                        acc[3] += (ai - pi + 1) + (aj - pj + 1);
                    }
                }
            }
        }
    }

    int index = 0;
    for (int i = 1; i < 5; ++i) {
        if (acc[i] > acc[index]) {
            index = i;
        }
    }

    os << x[index] << ' ';
    os << y[index] << ' ';
    os << s.getTile(ai, aj).getNestX() << ' ';
    os << s.getTile(ai, aj).getNestY() << ' ';
    /*
    os << ((s.getTile(ai -1, aj).value() != TL_FREE)? 1 : 0) << ' ';
    os << ((s.getTile(ai +1, aj).value() != TL_FREE)? 1 : 0) << ' ';
    os << ((s.getTile(ai, aj -1).value() != TL_FREE)? 1 : 0) << ' ';
    os << ((s.getTile(ai, aj +1).value() != TL_FREE)? 1 : 0) << '\n';
    */
    os << this->agentColisionProb(s, ai -1, aj) << ' ';
    os << this->agentColisionProb(s, ai +1, aj) << ' ';
    os << this->agentColisionProb(s, ai, aj -1) << ' ';
    os << this->agentColisionProb(s, ai, aj +1) << '\n';
}

float Map::agentColisionProb(const Tileset& s, int ai, int aj) {
    if (s.getTile(ai, aj).value() != TL_FREE) {
        return 0;
    }

    int c = -1;

    if(s.getTile(ai -1, aj).hasAgent()) {
        ++c;
    }

    if(s.getTile(ai +1, aj).hasAgent()) {
        ++c;
    }

    if(s.getTile(ai, aj -1).hasAgent()) {
        ++c;
    }

    if(s.getTile(ai, aj +1).hasAgent()) {
        ++c;
    }

    if(s.getTile(ai, aj).hasAgent()) {
        ++c;
    }

    return 1-(c*0.2);
}

void Map::setNestVector() {
    std::queue<int> queueI;
    std::queue<int> queueJ;
    Tileset& s = this->_tileset;
    for(int i = 0; i < s.height(); ++i) {
        for(int j = 0; j < s.width(); ++j) {
            Tile& t = this->_tileset.tile(i, j);
            t.color(TLC_WHITE);
            t.setNestX(0);
            t.setNestY(0);
            if (t.hasNest()) {
                t.color(TLC_BLACK);
                queueI.push(i);
                queueJ.push(j);
            }
        }
    }

    while(queueI.size() > 0 && queueJ.size() > 0) {
        const int i = queueI.front();
        queueI.pop();
        const int j = queueJ.front();
        queueJ.pop();

        if ((i-1) >= 0) {
            Tile& pt = this->_tileset.tile(i-1, j);
            if (pt.color() != TLC_BLACK) {
                pt.color(TLC_BLACK);
                pt.setNestX(1);
                pt.setNestY(0);
                queueI.push(i -1);
                queueJ.push(j);
            }
        }

        if ((i+1) < s.height()) {
            Tile& pt = this->_tileset.tile(i+1, j);
            if (pt.color() != TLC_BLACK) {
                pt.color(TLC_BLACK);
                pt.setNestX(-1);
                pt.setNestY(0);
                queueI.push(i +1);
                queueJ.push(j);
            }
        }

        if ((j-1) >= 0) {
            Tile& pt = this->_tileset.tile(i, j-1);
            if (pt.color() != TLC_BLACK) {
                pt.color(TLC_BLACK);
                pt.setNestX(0);
                pt.setNestY(1);
                queueI.push(i);
                queueJ.push(j-1);
            }
        }

        if ((j+1) < s.width()) {
            Tile& pt = this->_tileset.tile(i, j+1);
            if (pt.color() != TLC_BLACK) {
                pt.color(TLC_BLACK);
                pt.setNestX(0);
                pt.setNestY(-1);
                queueI.push(i);
                queueJ.push(j+1);
            }
        }
    }
}

void Map::proximity() {
    unsigned int unloaded = 0;
    unsigned int loaded = 0;
    for(auto it = this->_agents.begin(); it != this->_agents.end(); ++it) {
        std::queue<int> queueI;
        std::queue<int> queueJ;
        std::queue<unsigned int> queueValue;
        Tileset& s = it->perception();
        for(int i = 0; i < s.height(); ++i) {
            for(int j = 0; j < s.width(); ++j) {
                Tile& t = s.tile(i, j);
                t.color(TLC_WHITE);
            }
        }
        queueI.push(s.height()/2);
        queueJ.push(s.width()/2);
        queueValue.push(0);
        s.tile(s.height()/2, s.width()/2).color(TLC_BLACK);

        bool found = false;
        unsigned int max = s.width()/2;

        while(!found && queueI.size() > 0 && queueJ.size() > 0) {
            const int i = queueI.front();
            queueI.pop();
            const int j = queueJ.front();
            queueJ.pop();
            const unsigned int value = queueValue.front();
            queueValue.pop();

            Tile& t = s.tile(i, j);
            t.color(TLC_BLACK);
            if(it->hasFood()) {
                if (t.hasNest()) {
                    found = true;
                    loaded += (max - value +1);
                }
            }

            else {
                if (t.hasFood()) {
                    found = true;
                    unloaded += (max - value +1);
                }
            }

            if ((i-1) >= 0) {
                Tile& pt = s.tile(i-1, j);
                if (pt.color() != TLC_BLACK && pt.value() != TL_WALL) {
                    queueI.push(i -1);
                    queueJ.push(j);
                    queueValue.push(value +1);

                }
            }

            if ((i+1) < s.height()) {
                Tile& pt = s.tile(i+1, j);
                if (pt.color() != TLC_BLACK && pt.value() != TL_WALL) {
                    queueI.push(i +1);
                    queueJ.push(j);
                    queueValue.push(value +1);
                }
            }

            if ((j-1) >= 0) {
                Tile& pt = s.tile(i, j-1);
                if (pt.color() != TLC_BLACK && pt.value() != TL_WALL) {
                    queueI.push(i);
                    queueJ.push(j-1);
                    queueValue.push(value +1);
                }
            }

            if ((j+1) < s.width()) {
                Tile& pt = s.tile(i, j+1);
                if (pt.color() != TLC_BLACK && pt.value() != TL_WALL) {
                    queueI.push(i);
                    queueJ.push(j+1);
                    queueValue.push(value +1);
                }
            }
        }
    }
    Stats::stats()->setEvent(EV_PROX_LOAD, loaded);
    Stats::stats()->setEvent(EV_PROX_UNLOAD, unloaded);
}
