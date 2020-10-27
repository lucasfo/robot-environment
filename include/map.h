#ifndef __MAP_H
#define __MAP_H

#include <iostream>
#include <tile.h>
#include <agent.h>
#include <action.h>
#include <printer.h>
#include <task.h>
#include <stats.h>
#include <tileset.h>
#include <queue>
#include <list>

class Map {
    private:
        Tileset _tileset;

        // To multiple agents, this should be a list
        std::list<Agent> _agents;
        std::list<int> footPrintI;
        std::list<int> footPrintJ;

        bool _printAsLine;
        const Tile& getTile(const int i, const int j) const;
        const Tile& getTile(const Agent& a, const int iInc, const int jInc) const;
        const Tile& getTile(const Agent& a) const;
        void moveAgent(Agent& agent);
        void feedAgent(const Task& task, Agent& agent);
        void invalidate();
        void free();
        void compactPerception(std::ostream& os, const Tileset& t);
        void setNestVector();
    public:
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        void proximity();
        bool perform(const std::list<Action>& action, const Task& task);
        void percept();
        bool valid() const;
        bool updateAgent(AgentModel& model, std::list<int> i, std::list<int> j, bool printAsLine);
        float agentColisionProb(const Tileset&, int ai, int aj);
        Map();
};

std::ostream& operator<<(std::ostream& os, const Map& map);
std::istream& operator>>(std::istream& is, Map& map);
//Map& operator=(const Map& map);

#endif
