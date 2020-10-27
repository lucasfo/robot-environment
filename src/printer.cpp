#include <printer.h>

Printer* Printer::_printer = NULL;
Printer* Printer::_logger = NULL;

Printer::Printer(std::ostream& stream): os(stream) {
    this->_level = LG_ERROR;
}

Printer* const Printer::print() {
    if (!_printer) {
        _printer = new Printer(std::cout);
    }

    return _printer;
}

Printer* const Printer::log() {
    if (!_logger) {
        _logger = new Printer(std::cerr);
    }

    return _logger;
}

bool Printer::setLogLevel(const int level) {
    switch(level) {
        case LG_DEBUG:
        case LG_INFO:
        case LG_WARN:
        case LG_ERROR:
            this->_level = level;
            return true;
        default:
            this->_level = LG_NONE;
            return false;
    }
}

void Printer::debug(const std::string msg) const {
    if (this->_level < (LG_DEBUG +1)) {
        this->line("[DEBUG]: " + msg);
    }
}

void Printer::info(const std::string msg) const {
    if (this->_level < (LG_INFO +1)) {
        this->line("[INFO]: " + msg);
    }
}

void Printer::warn(const std::string msg) const {
    if (this->_level < (LG_WARN +1)) {
        this->line("[WARN]: " + msg);
    }
}

void Printer::error(const std::string msg) const {
    if (this->_level < (LG_ERROR +1)) {
        this->line("[ERROR]: " + msg);
    }
}

void Printer::line(const std::string msg) const {
    this->os << msg << std::endl;
}

std::ostream& Printer::stream() {
    return os;
}

void Printer::help(const char* argv0) const {
    this->os << "Correct use: " << argv0 << " [OPTIONS]" << std::endl;
    this->os << "Simulator for agent performing surviving task." << std::endl;
    this->os << "The simulator expects the:" << std::endl;
    this->os << "\tTask description: <mode> <food power> <decay>" << std::endl;
    this->os << "\tAgent Model description: <vision mode> <vision range> <max> <cost> <foot print>" << std::endl;
    this->os << "\tMap description: <width> <height> <tiles>" << std::endl;
    this->os << "\tList of agents: <number of agents> <agent coordinates>" << std::endl;
    this->os << "\tSequence of actions" << std::endl;
    this->os << "The simulator expects this in the given order, unless options are used" << std::endl;
    this->os << "The available options are:" << std::endl;
    this->os << "-a <file>: File with agent description." << std::endl;
    this->os << "\tIf not provided, reads from standart input." << std::endl;
    this->os << "\tIf file is set to '-', reads from standart input." << std::endl;
    this->os << "-d <level>: Defines the log level." << std::endl;
    this->os << "\tInvalid log levels will emit no logs" << std::endl;
    this->os << "-h : Show this help info." << std::endl;
    this->os << "-i <file>: Input file (for actions)" << std::endl;
    this->os << "\tIf not provided, reads from standart input." << std::endl;
    this->os << "\tIf file is set to '-', reads from standart input." << std::endl;
    this->os << "-l <file>: Log file." << std::endl;
    this->os << "\tIf not provided, print logs on error output." << std::endl;
    this->os << "\tIf file is set to '-', print logs on error output." << std::endl;
    this->os << "-m <file>: File with map description." << std::endl;
    this->os << "\tIf not provided, reads from standart input." << std::endl;
    this->os << "\tIf file is set to '-', reads from standart input." << std::endl;
    this->os << "-o <file>: Output File (for perceptions)." << std::endl;
    this->os << "\tIf not provided, prints on standart output." << std::endl;
    this->os << "\tIf file is set to '-', print logs on standart output." << std::endl;
    this->os << "-r <mode>: Printing mode of statistical report." << std::endl;
    this->os << "\tAvailable modes are: csv, tsv, ssv" << std::endl;
    this->os << "\tIs posible to add a '+' to print header: Ex csv+" << std::endl;
    this->os << "\tThe default option is: ssv+" << std::endl;
    this->os << "-s <file>: File with swarm description" << std::endl;
    this->os << "\tIf not provided, reads from standart input." << std::endl;
    this->os << "\tIf file is set to '-', reads from standart input." << std::endl;
    this->os << "-t <file>: File with task description" << std::endl;
    this->os << "\tIf not provided, reads from standart input." << std::endl;
    this->os << "\tIf file is set to '-', reads from standart input." << std::endl;
}
