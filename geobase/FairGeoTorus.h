#ifndef FAIRGEOTORUS_H
#define FAIRGEOTORUS_H

#include "FairGeoBasicShape.h"          // for FairGeoBasicShape

#include "Riosfwd.h"                    // for fstream
#include "Rtypes.h"                     // for FairGeoPcon::Class, Bool_t, etc

#include <iosfwd>                       // for fstream

class FairGeoTransform;
class FairGeoVolume;
class TArrayD;

class FairGeoTorus : public FairGeoBasicShape
{
  public:
    FairGeoTorus();
    ~FairGeoTorus();
    TArrayD* calcVoluParam(FairGeoVolume*);
    void calcVoluPosition(FairGeoVolume*,
                          const FairGeoTransform&,const FairGeoTransform&);
    Int_t readPoints(fstream* pFile,FairGeoVolume* volu);
    Bool_t writePoints(fstream*,FairGeoVolume*);
    void printPoints(FairGeoVolume* volu);

    ClassDef(FairGeoTorus,0) // class for geometry shape TORUS
};

#endif  /* !FAIRGEOTORUS_H */
