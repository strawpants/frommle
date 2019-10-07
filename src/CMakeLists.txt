LIST(APPEND COREHEADERS core/GuideBase.hpp core/TimeGuide.hpp core/GArrayBase.hpp core/GOperatorBase.hpp
        io/ArchiveBase.hpp core/PhysQuantityBase.hpp core/seqGenerator.hpp core/GArrayBase.hpp core/IndexGuide.hpp core/Exceptions.hpp core/GuidePack.hpp core/MarrayEig.hpp core/Logging.hpp )

LIST(APPEND COREOBJS core/GuideBase.cpp core/GOperatorBase.cpp core/GuidePack.cpp)

LIST(APPEND COREHEADERS core/UserSettings.hpp core/Hyperslab.hpp core/GuideAppender.hpp)
LIST(APPEND COREOBJS core/UserSettings.cpp)

LIST(APPEND COREHEADERS core/TreeNode.hpp)
LIST(APPEND COREOBJS core/TreeNode.cpp)

LIST(APPEND SHHEADERS sh/SHGuide.hpp sh/Legendre_nm.hpp sh/SHanalysis.hpp sh/SHfunctions.hpp sh/Ynm.hpp)
LIST(APPEND SHOBJS sh/Legendre_nm.cpp sh/SHGuide.cpp)

LIST(APPEND GEOSPATHEADERS geometry/GeoGrid.hpp geometry/OGRGuide.hpp geometry/OGR2boost.hpp geometry/OGRiteratorBase.hpp geometry/fibonacciGrid.hpp geometry/geomOperator.hpp)
LIST(APPEND GEOSPATOBJS geometry/GeoGrid.cpp geometry/fibonacciGrid.cpp geometry/OGRGuide.cpp)

LIST(APPEND IOHEADERS io/getpass.hpp io/OGRArchive.hpp  io/Group.hpp  io/OGRIOArchives.hpp io/LineBuffer.hpp io/NetCDFIO.hpp io/CFConventions.hpp)
LIST(APPEND IOOBJS io/getpass.cpp io/OGRArchive.cpp io/Group.cpp io/OGRIOArchives.cpp io/LineBuffer.cpp io/NetCDFIO.cpp io/CFConventions.cpp)


LIST(APPEND SEAHEADERS sealevel/OceanFunction.hpp)
#LIST(APPEND SEAOBJS sealevel/)

LIST(APPEND EARTHHEADERS earthmodels/EarthConstants.hpp)
LIST(APPEND EARTHOBJS earthmodels/EarthConstants.cpp)
#LIST(APPEND SEAOBJS sealevel/)

if (PYTHON)
    #add python wrapper routines
    list(APPEND CXXPYWRAPS pybindings/coreBindings.cpp pybindings/coreBindings.hpp pybindings/datetimeconversion.hpp pybindings/tupleconversion.hpp pybindings/tupleconversion.cpp pybindings/earthBindings.cpp)
    list(APPEND CXXPYWRAPS pybindings/SHBindings.cpp )
    list(APPEND CXXPYWRAPS pybindings/geometryBindings.cpp )
    list(APPEND CXXPYWRAPS pybindings/GuidePackBindings.cpp )
    add_library(${FROMMLEPYLIB} SHARED ${CXXPYWRAPS} )
endif()

add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS}
        ${SHHEADERS} ${SHOBJS}
        ${GEOSPATHEADERS} ${GEOSPATOBJS}
        ${IOHEADERS} ${IOOBJS}
        ${SEAHEADERS} ${EARTHHEADERS} ${EARTHOBJS} )
target_include_directories(${FROMMLELIB} PUBLIC ${Boost_INCLUDE_DIR} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})

