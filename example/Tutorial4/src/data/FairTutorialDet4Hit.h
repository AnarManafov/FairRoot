#ifndef FAIRTUTORIALDERT4HIT_H
#define FAIRTUTORIALDET4HIT_H 1

#include "FairHit.h"                    // for FairHit

#include "Rtypes.h"                     // for FairTutorialDet4Hit::Class, etc

class TVector3;

class FairTutorialDet4Hit : public FairHit
{

  public:

    /** Default constructor **/
    FairTutorialDet4Hit();
    FairTutorialDet4Hit(Int_t detID, Int_t mcindex, TVector3& pos, TVector3& dpos);

    /** Destructor **/
    virtual ~FairTutorialDet4Hit();

  private:

    FairTutorialDet4Hit(const FairTutorialDet4Hit&);
    FairTutorialDet4Hit operator=(const FairTutorialDet4Hit&);

    ClassDef(FairTutorialDet4Hit,1);

};
#endif
