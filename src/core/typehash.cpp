#include "core/typehash.hpp"

namespace frommle{
	namespace core{

		//constexpr int GuideBase::ndim;
	
		std::ostream &operator<<(std::ostream &os, typehash const &m) {
			return m.write(os);
		}
}
}
