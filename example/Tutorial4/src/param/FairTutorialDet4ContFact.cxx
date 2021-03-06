#include "FairTutorialDet4ContFact.h"

#include "FairRuntimeDb.h"              // for FairRuntimeDb
#include "FairTutorialDet4GeoPar.h"     // for FairTutorialDet4GeoPar
#include "FairTutorialDet4MisalignPar.h"

#include "TList.h"                      // for TList
#include "TString.h"                    // for TString

#include <string.h>                     // for strcmp, NULL

class FairParSet;

ClassImp(FairTutorialDet4ContFact)

static FairTutorialDet4ContFact gFairTutorialDet4ContFact;

FairTutorialDet4ContFact::FairTutorialDet4ContFact()
  : FairContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="FairTutorialDet4ContFact";
  fTitle="Factory for parameter containers in libTutorial1";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void FairTutorialDet4ContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the Tutorial1 library.
  */

  FairContainer* p= new FairContainer("FairTutorialDet4GeoPar",
                                      "FairTutorialDet4 Geometry Parameters",
                                      "TestDefaultContext");
  p->addContext("TestNonDefaultContext");

  containers->Add(p);

  FairContainer* p1= new FairContainer("FairTutorialDet4MissallignPar",
                                       "FairTutorialDet4 Missallignment Parameters",
                                       "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");

  containers->Add(p1);
}

FairParSet* FairTutorialDet4ContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
      For an actual context, which is not an empty string and not
      the default context
      of this container, the name is concatinated with the context.
  */
  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"FairTutorialDet4GeoPar")==0) {
    p=new FairTutorialDet4GeoPar(c->getConcatName().Data(),
                                 c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"FairTutorialDet4MissallignPar")==0) {
    p=new FairTutorialDet4MisalignPar(c->getConcatName().Data(),
                                      c->GetTitle(),c->getContext());
  }
  return p;
}
