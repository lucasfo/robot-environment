#ifndef __AGENT_H
#define __AGENT_H

#include <iostream>
#include <printer.h>
#include <tileset.h>
#include <vision.h>
#include <agentmodel.h>

class Agent {
    private:
        int _i;
        int _j;
        int _lastI;
        int _lastJ;
        int _power;
        AgentModel _model;
        Tileset _perception;
        bool _valid;
        bool _lockUndo;
        bool _toUndo;
        bool _hasFood;

    public:
        Agent(const AgentModel& m, int i, int j);
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        bool valid() const;
        int i() const;
        int j() const;
        void incrementPosition(const int iInc, const int jInc);
        void undoIncrement();
        int power() const;
        int powerUp(const int inc);
        int doAction();
        bool alive() const;
        Tileset& perception();
        int range() const;
        int vision() const;
        unsigned int footPrint() const;
        const AgentModel& model() const;
        bool setToUndo(bool toUndo);
        bool hasToUndo() const;
        bool hasFood() const;
        void getFood();
        void loseFood();
        void freeUndo();
};

std::ostream& operator<<(std::ostream& os, const Agent& agent);
std::istream& operator>>(std::istream& is, Agent& agent);

//Agent& operator=(const Agent& agent);
#endif
