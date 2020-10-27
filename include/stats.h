#ifndef __STATS_H
#define __STATS_H

#include <iostream>
#include <printer.h>

enum EnRep {RP_NONE, RP_CSV , RP_TSV, RP_SSV, RP_CSVP , RP_TSVP, RP_SSVP, RP_END};
enum EnEvt {EV_NONE, EV_FEED , EV_COLIDE, EV_ACT, EV_LOAD, EV_ACT_LOAD, EV_ACT_UNLOAD, EV_EXPLORE, EV_EXPLOIT, EV_PROX_LOAD, EV_PROX_UNLOAD, EV_END};

class Stats {
    private:
        Stats();
        Stats(const Stats& logger);
        Stats& operator=(const Stats& logger);
        int _mode;
        static const std::string _modes[RP_END];
        static const std::string _eventHeaders[EV_END];
        static Stats* _stats;
        unsigned int _events[EV_END];
    public:
        static Stats* const stats();
        bool setReportMode(const char* mode);
        bool event(const int event);
        bool setEvent(const int event, const unsigned int value);
        void report();
        void reboot();
};

#endif
