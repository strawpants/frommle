#include "earthmodels/EarthConstants.hpp"

namespace frommle{
    namespace earthmodels{
        //Note we need to define the member variables in the cpp file so they are instantated 
        constexpr double WGS84::a;
         constexpr double WGS84::finv;
         constexpr double WGS84::omegaRate;
         constexpr double WGS84::GM;
    }
}

