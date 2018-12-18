LIST(APPEND COREHEADERS core/GuideBase.hpp core/TimeGuide.hpp core/GArrayBase.hpp core/OperatorBase.hpp core/InputArchiveBase.hpp
        core/OutputArchiveBase.hpp core/UnitBase.hpp core/seqGenerator.hpp core/GArrayBase.hpp core/IndexGuide.hpp )
LIST(APPEND COREOBJS core/TimeGuide.cpp)

LIST(APPEND SHHEADERS sh/SHGuide.hpp sh/Legendre_nm.hpp)
LIST(APPEND SHOBJS sh/Legendre_nm.cpp)

if (PYTHON)
    #add python wrapper routines
    list(APPEND CXXPYWRAPS pybindings/coreBindings.cpp)
    list(APPEND CXXPYWRAPS pybindings/SHBindings.cpp )
    add_library(${FROMMLEPYLIB} SHARED ${CXXPYWRAPS}  )
endif()

add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS} ${SHHEADERS} ${SHOBJS} )
target_include_directories(${FROMMLELIB} PUBLIC ${Boost_INCLUDE_DIR} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})

