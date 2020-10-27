#ifndef __AGENT_MODEL_H
#define __AGENT_MODEL_H

#include <iostream>
#include <printer.h>
#include <vision.h>

class AgentModel {
    private:
        bool _valid;
        int _i;
        int _j;
        Vision _vision;
        int _maxPower;
        int _actionPower;
        unsigned int _footPrint;

    public:
        AgentModel();
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        bool valid() const;
        int maxPower() const;
        int actionPower() const;
        int range() const;
        const Vision& vision() const;
        unsigned int footPrint() const;
};

std::ostream& operator<<(std::ostream& os, const AgentModel& model);
std::istream& operator>>(std::istream& is, AgentModel& model);

//Agent& operator=(const Agent& agent);
#endif
