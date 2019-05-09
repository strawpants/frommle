LIST(APPEND COREHEADERS core/GuideBase.hpp core/TimeGuide.hpp core/GArrayBase.hpp core/GOperatorBase.hpp core/InputArchiveBase.hpp
    core/OutputArchiveBase.hpp core/PhysQuantityBase.hpp core/seqGenerator.hpp core/GArrayBase.hpp core/IndexGuide.hpp core/Exceptions.hpp core/GuidePack.hpp core/MarrayEig.hpp core/Logging.hpp )

LIST(APPEND COREOBJS core/TimeGuide.cpp )

LIST(APPEND COREHEADERS core/UserSettings.hpp)
LIST(APPEND COREOBJS core/UserSettings.cpp)

LIST(APPEND SHHEADERS sh/SHGuide.hpp sh/Legendre_nm.hpp sh/SHanalysis.hpp sh/SHfunctions.hpp)
LIST(APPEND SHOBJS sh/Legendre_nm.cpp sh/SHGuide.cpp)

LIST(APPEND GEOSPATHEADERS geometry/GeoGrid.hpp geometry/OGRGuide.hpp)
LIST(APPEND GEOSPATOBJS geometry/GeoGrid.cpp )

LIST(APPEND IOHEADERS io/getpass.hpp io/OGRIArchive.hpp)
LIST(APPEND IOOBJS io/getpass.cpp io/OGRIArchive.cpp)

LIST(APPEND SEAHEADERS sealevel/OceanFunction.hpp)
#LIST(APPEND SEAOBJS sealevel/)


if (PYTHON)
    #add python wrapper routines
    list(APPEND CXXPYWRAPS pybindings/coreBindings.cpp pybindings/coreBindings.hpp pybindings/tupleconversion.hpp)
    list(APPEND CXXPYWRAPS pybindings/SHBindings.cpp )
    add_library(${FROMMLEPYLIB} SHARED ${CXXPYWRAPS} )
endif()

add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS}
        ${SHHEADERS} ${SHOBJS}
        ${GEOSPATHEADERS} ${GEOSPATOBJS}
        ${IOHEADERS} ${IOOBJS}
        ${SEAHEADERS})
target_include_directories(${FROMMLELIB} PUBLIC ${Boost_INCLUDE_DIR} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})

