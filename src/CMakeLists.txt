# source files are to be added here

LIST(APPEND COREHEADERS core/frommle.hpp core/GuideBase.hpp core/TimeGuide.hpp core/GArrayBase.hpp core/GOperatorBase.hpp
        io/ArchiveBase.hpp core/PhysQuantityBase.hpp core/seqGenerator.hpp core/GArrayBase.hpp core/IndexGuide.hpp core/Exceptions.hpp core/GuidePack.hpp core/MarrayEig.hpp core/Logging.hpp )

LIST(APPEND COREHEADERS core/Attributes.hpp)
LIST(APPEND COREHEADERS core/typehash.hpp)
LIST(APPEND COREHEADERS core/PhysQuantGuide.hpp)
LIST(APPEND COREOBJS core/typehash.cpp core/GOperatorBase.cpp core/GuidePack.cpp core/TimeGuide.cpp core/frommle.cpp)

LIST(APPEND COREHEADERS core/UserSettings.hpp  core/GArrayDiag.hpp core/Hyperslab.hpp core/GuideAppender.hpp )
LIST(APPEND COREOBJS core/UserSettings.cpp core/Hyperslab.cpp)

LIST(APPEND COREHEADERS core/TreeNode.hpp)
LIST(APPEND COREOBJS core/TreeNode.cpp)

LIST(APPEND SHHEADERS sh/SHGuide.hpp sh/Legendre_nm.hpp sh/Legendre.hpp
        sh/SHanalysis.hpp sh/SHfunctions.hpp sh/Ynm.hpp
        sh/SHisoOperator.hpp)
LIST(APPEND SHOBJS sh/Legendre_nm.cpp sh/Legendre.cpp sh/SHGuide.cpp)

LIST(APPEND GEOSPATHEADERS geometry/GeoGrid.hpp geometry/OGRGuide.hpp geometry/OGR2boost.hpp geometry/OGRiteratorBase.hpp geometry/GuideMakerTools.hpp geometry/geomOperator.hpp)
LIST(APPEND GEOSPATOBJS geometry/GeoGrid.cpp geometry/GuideMakerTools.cpp geometry/OGRGuide.cpp)

LIST(APPEND IOHEADERS io/getpass.hpp io/OGRArchive.hpp  io/Group.hpp  io/OGRIOArchives.hpp io/LineBuffer.hpp io/NetCDFIO.hpp io/Conventions.hpp)
LIST(APPEND IOOBJS io/getpass.cpp io/OGRArchive.cpp io/Group.cpp io/OGRIOArchives.cpp io/LineBuffer.cpp io/NetCDFIO.cpp io/Conventions.cpp)


LIST(APPEND SEAHEADERS sealevel/OceanFunction.hpp)
#LIST(APPEND SEAOBJS sealevel/)

LIST(APPEND EARTHHEADERS earthmodels/EarthConstants.hpp)
LIST(APPEND EARTHOBJS earthmodels/EarthConstants.cpp)
#LIST(APPEND SEAOBJS sealevel/)


add_library(${FROMMLELIB} SHARED ${COREOBJS} ${COREHEADERS}
        ${SHHEADERS} ${SHOBJS}
        ${GEOSPATHEADERS} ${GEOSPATOBJS}
        ${IOHEADERS} ${IOOBJS}
        ${SEAHEADERS} ${EARTHHEADERS} ${EARTHOBJS} )
target_include_directories(${FROMMLELIB} PUBLIC ${Boost_INCLUDE_DIR} )
#set_target_properties(${FROMMLELIB} PROPERTIES PUBLIC_HEADER ${COREHEADERS})

