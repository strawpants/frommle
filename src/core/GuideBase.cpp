#include "core/GuideBase.hpp"

namespace frommle{
	namespace guides{

		//constexpr int GuideBase::ndim;
	
		std::ostream &operator<<(std::ostream &os, frommle::guides::typehash const &m) {
			return m.write(os);
		}
}
}
