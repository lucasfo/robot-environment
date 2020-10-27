#ifndef __TASK_H
#define __TASK_H

#include <iostream>

enum EnTask {TK_NONE, TK_SURV_ST, TK_END};

class Task {
    private:
        EnTask _type;
        bool _valid;
        int _foodPower;
        int _decay;
    public:
        Task();
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        EnTask type() const;
        bool valid() const;
        int foodPower() const;
        int decay() const;
};

std::ostream& operator<<(std::ostream& os, const Task& task);
std::istream& operator>>(std::istream& is, Task& task);

//Task& operator=(const Task& task);
#endif
