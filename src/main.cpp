#include <iostream>
#include <fstream>
#include <unistd.h>
#include <action.h>
#include <map.h>
#include <printer.h>
#include <task.h>
#include <stats.h>
#include <list>

int main(int argc, char* argv[]) {
    // Handles argc argv
    Map map;
    AgentModel model;
    Task task;

    long unsigned int nAgents;
    std::list<int> agentI;
    std::list<int> agentJ;

    std::fstream fs, fin, fout, ferr;
    std::string inPath = "";
    std::string outPath = "";
    std::string errPath = "";

    bool modelInInput = true;
    bool swarmInInput = true;
    bool mapInInput = true;
    bool taskInInput = true;
    bool printAsLine = false;
    int opt;
    Printer::print()->setLogLevel(-1);
    while ((opt = getopt(argc, argv, "a:d:hi:l:m:no:s:r:t:")) != -1) {
        switch(opt) {
            case 'a':
                modelInInput = false;
                fs.open(optarg, std::fstream::in);
                fs >> model;
                fs.close();
                break;
            case 'd':
                if(!Printer::log()->setLogLevel(atoi(optarg))) {
                    Printer::log()->warn("Log disable. Invalid log level");
                    Printer::log()->warn("No further loggind will happen");
                }
                break;
            case 'h':
                Printer::print()->help(argv[0]);
                return 0;
                break;
            case 'i':
                inPath = optarg;
                break;
            case 'l':
                errPath = optarg;
                break;
            case 'm':
                mapInInput = false;
                fs.open(optarg, std::fstream::in);
                fs >> map;
                fs.close();
                break;
            case 'n':
                printAsLine = true;
                break;
            case 'o':
                outPath = optarg;
                break;
            case 'r':
                if(Stats::stats()->setReportMode(optarg)) {
                    std::string msg = "Report mode set to: ";
                    msg += optarg;
                    Printer::log()->info(msg);
                }

                else {
                    Printer::log()->warn("Invalid statistics report mode.");
                    Printer::log()->warn("Report mode set to NONE");
                }
                break;
            case 's':
                swarmInInput = false;
                fs.open(optarg, std::fstream::in);
                fs >> nAgents;
                for(long unsigned int i = 0; i < nAgents; ++i) {
                    int ci, cj;
                    fs >> ci >> cj;
                    agentI.push_back(ci);
                    agentJ.push_back(cj);
                }
                fs.close();
                break;
            case 't':
                taskInInput = false;
                fs.open(optarg, std::fstream::in);
                fs >> task;
                fs.close();
                break;
        }
    }

    if (!(inPath == "" || inPath == "-")) {
        fin.open(inPath.c_str(), std::fstream::in);
        std::cin.rdbuf(fin.rdbuf());
    }

    if (!(outPath == "" || outPath == "-")) {
        fout.open(outPath.c_str(), std::fstream::out);
        std::cout.rdbuf(fout.rdbuf());
    }

    if (!(errPath == "" || errPath == "-")) {
        ferr.open(errPath.c_str(), std::fstream::out);
        std::cerr.rdbuf(ferr.rdbuf());
    }


    if (taskInInput) {
        std::cin >> task;
    }

    if (modelInInput) {
        std::cin >> model;
    }

    if (mapInInput) {
        std::cin >> map;
    }

    if (swarmInInput) {
        std::cin >> nAgents;
        for(long unsigned int i = 0; i < nAgents; ++i) {
            int ci, cj;
            std::cin >> ci >> cj;
            agentI.push_back(ci);
            agentJ.push_back(cj);
        }
    }

    bool abort = false;

    if(!model.valid()) {
        abort = true;
        Printer::log()->error("Invalid agent model description.");
    }

    else {
        Printer::log()->info("Agent model description read with sucess.");
    }

    if(!task.valid()) {
        abort = true;
        Printer::log()->error("Invalid task description.");
    }

    else {
        Printer::log()->info("Task description read with sucess.");
    }

    if(!map.valid()) {
        abort = true;
        Printer::log()->error("Invalid map description.");
    }

    else {
        Printer::log()->info("Map description read with sucess.");
    }

    if(abort) {
        return 1;
    }

    if(!map.updateAgent(model, agentI, agentJ, printAsLine)) {
        Printer::log()->error("Agent could not be inserted in the map.");
        return 1;
    }

    else {
        Printer::log()->info("Agent positioned in map with sucess.");
    }

    Map oldMap = map;
    Action action;
    std::list<Action> actions;
    // std::cerr << agent << std::endl;
    // std::cerr << map << std:: endl;
    bool alive = true;
    while (!abort) {
        map.percept();
        actions.clear();
        while (std::cin.good() && alive) {
            std::cin >> action;
            Printer::log()->info("Read action.");
            if (!action.valid()) {
                Printer::log()->warn("Invalid action, setting to STAY.");
                action.set('s');
            }

            if (action.value() == AC_ABORT) {
                alive = false;
                abort = true;
                Printer::log()->info("Ending simulation by ABORT action");
            }

            if (action.value() == AC_FINISH) {
                alive = false;
                Printer::log()->info("Reloading simulation by FINISH action");
            }

            actions.push_back(action);

            if (actions.size() == nAgents && alive) {
                alive = map.perform(actions, task);
                map.percept();
                actions.clear();
            }

        }

        if (!std::cin.good()) {
            Printer::log()->info("Ending simulation by closure of the input stream");
            abort = true;
        }

        map.proximity();
        Stats::stats()->report();
        Stats::stats()->reboot();
        map = oldMap;
        alive = true;
    }

    return 0;
}
