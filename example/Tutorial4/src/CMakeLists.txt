# Create a library called "libTutorial1" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/example/Tutorial4/src
${CMAKE_SOURCE_DIR}/example/Tutorial4/src/mc
${CMAKE_SOURCE_DIR}/example/Tutorial4/src/param
${CMAKE_SOURCE_DIR}/example/Tutorial4/src/data
${CMAKE_SOURCE_DIR}/example/Tutorial4/src/reco
${CMAKE_SOURCE_DIR}/example/Tutorial4/src/tools
${CMAKE_SOURCE_DIR}/example/mcstack
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#Put here your sourcefiles
mc/FairTutorialDet4.cxx
mc/FairTutorialDet4Geo.cxx

param/FairTutorialDet4ContFact.cxx
param/FairTutorialDet4GeoPar.cxx
param/FairTutorialDet4MisalignPar.cxx

data/FairTutorialDet4Point.cxx
data/FairTutorialDet4Hit.cxx

reco/FairTutorialDet4HitProducerIdealMisalign.cxx
reco/FairTutorialDet4StraightLineFitter.cxx
reco/FairTutorialDet4MilleWriter.cxx

tools/FairTutorialDet4GeoHandler.cxx
)

set(NO_DICT_SRCS tools/Mille.cc)
Set(HEADERS )
Set(LINKDEF Tutorial4LinkDef.h)
Set(LIBRARY_NAME Tutorial4)
Set(DEPENDENCIES Base MCStack GeoBase ParBase VMC Physics Cint Core)

GENERATE_LIBRARY()

