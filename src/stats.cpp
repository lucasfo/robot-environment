#include <stats.h>

Stats* Stats::_stats = NULL;
const std::string Stats::_modes[RP_END] = {
    "none"
    , "csv"
    , "tsv"
    , "ssv"
    , "csv+"
    , "tsv+"
    , "ssv+"
};

const std::string Stats::_eventHeaders[EV_END] = {
    ""
    , "nFoods"
    , "nColisions"
    , "nActions"
    , "nLoads"
    , "nActionsLoaded"
    , "nActionsUnLoaded"
    , "nTilesDiscovered"
    , "nActionsExploit"
    , "nLoadedProximity"
    , "nUnloadedProximity"
};

Stats::Stats() {
    this->_mode = RP_SSVP;
    this->reboot();
}

Stats* const Stats::stats() {
    if (!_stats) {
        _stats = new Stats();
    }

    return _stats;
}

void Stats::reboot() {
    for (int i = 0; i < EV_END; ++i) {
        this->_events[i] = 0;
    }
}
bool Stats::setReportMode(const char* input) {
    std::string mode(input);
    for (int i = 0; i < RP_END; ++i) {
        if (input == Stats::_modes[i]) {
            this->_mode = i;
            return true;
        }
    }

    this->_mode = RP_NONE;
    return false;
}

bool Stats::event(const int event) {
    if (event > 0 && event < EV_END) {
        ++this->_events[event];
        return true;
    }

    return false;
}

bool Stats::setEvent(const int event, const unsigned int value) {
    if (event > 0 && event < EV_END) {
        this->_events[event] = value;
        return true;
    }

    return false;
}

void Stats::report() {
    char sep;
    bool header;

    switch(this->_mode) {
        case RP_CSV:
        case RP_CSVP:
            sep = ',';
            break;
        case RP_SSV:
        case RP_SSVP:
            sep = ';';
            break;
        case RP_TSV:
        case RP_TSVP:
            sep = '\t';
            break;
        default:
            sep = '\0';
            break;
    }

    switch(this->_mode) {
        case RP_CSVP:
        case RP_SSVP:
        case RP_TSVP:
            header = true;
            break;
        default:
            header = false;
            break;
    }

    std::ostream& os = Printer::log()->stream();

    if (header) {
        os << this->_eventHeaders[1];
        for (int i = 2; i < EV_END; ++i) {
            os << sep << this->_eventHeaders[i];
        }
        os << std::endl;
    }

    os << this->_events[1];
    for (int i = 2; i < EV_END; ++i) {
        os << sep << this->_events[i];
    }
    os << std::endl;
}
