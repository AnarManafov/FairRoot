#ifndef Target_H
#define Target_H

#include "FairModule.h"                 // for FairModule

#include "Rtypes.h"                     // for FairTarget::Class, ClassDef, etc

class FairTarget : public FairModule
{
  public:
    FairTarget(const char* name, const char* Title="PND Target");
    FairTarget();
    virtual ~FairTarget();
    virtual void ConstructGeometry();
    ClassDef(FairTarget,1)

};

#endif //Target_H

