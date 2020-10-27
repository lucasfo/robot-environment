#ifndef __VISION_H
#define __VISION_H

#include <iostream>
#include <tileset.h>

enum EnVision {VI_NONE, VI_BOX, VI_BFS, VI_BOX_BFS, VI_END};

class Vision {
    private:
        int _range;
        EnVision _vision;
        bool _valid;
    public:
        Vision();
        void toOutputStream(std::ostream& os) const;
        void fromInputStream(std::istream& is);
        bool valid() const;
        EnVision mode() const;
        int range() const;
};

std::ostream& operator<<(std::ostream& os, const Vision& vision);
std::istream& operator>>(std::istream& is, Vision& vision);

#endif
