#ifndef __PRINTER_H
#define __PRINTER_H

#include <iostream>

enum EnLog {LG_DEBUG = 0, LG_INFO, LG_WARN, LG_ERROR, LG_NONE};

class Printer {
    private:
        Printer();
        Printer(std::ostream& os);
        Printer(const Printer& logger);
        Printer& operator=(const Printer& logger);
        int _level;
        static Printer* _printer;
        static Printer* _logger;
        std::ostream& os;
    public:
        static Printer* const log();
        static Printer* const print();
        bool setLogLevel(const int level);
        void debug(const std::string msg) const;
        void info(const std::string msg) const;
        void warn(const std::string msg) const;
        void error(const std::string msg) const;
        void line(const std::string msg) const;
        void help(const char* argv0) const;
        std::ostream& stream();
};

#endif
