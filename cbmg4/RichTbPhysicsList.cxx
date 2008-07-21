//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// Rich advanced example for Geant4
// RichTbPhysicsList.cc for Rich of LHCb
// History:
// Created: Sajan Easo (Sajan.Easo@cern.ch)
// Revision and changes: Patricia Mendez (Patricia.Mendez@cern.ch)
/////////////////////////////////////////////////////////////////////////////
#include "RichTbPhysicsList.h"

#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTable.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ParticleTable.hh"
#include "G4UserPhysicsListMessenger.hh"
#include "G4UImanager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4UnitsTable.hh"

//#include <iomanip>


RichTbPhysicsList::RichTbPhysicsList()
  : G4VUserPhysicsList() {

  G4cout<<" Now define the physics List"<<G4endl;
//  rConfigPh = RConfig;


    defaultCutValue = 0.1*mm;

  // pointer to the particle table
  theParticleTable = G4ParticleTable::GetParticleTable();
  theParticleIterator = theParticleTable->GetIterator();

}
//RichTbPhysicsList::RichTbPhysicsList() :G4VUserPhysicsList(){
//  // pointer to the particle table
//  theParticleTable = G4ParticleTable::GetParticleTable();
//  theParticleIterator = theParticleTable->GetIterator();


//}

RichTbPhysicsList::~RichTbPhysicsList() {}

void RichTbPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.

 // ConstructBosons();
//  ConstructLeptons();
//  ConstructMesons();
//  ConstructBaryons();
 G4OpticalPhoton::OpticalPhotonDefinition();
}

void RichTbPhysicsList::ConstructBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void RichTbPhysicsList::ConstructLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void RichTbPhysicsList::ConstructMesons()
{
 //  mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
}

void RichTbPhysicsList::ConstructBaryons()
{
//  barions
   G4Proton::ProtonDefinition();
   G4AntiProton::AntiProtonDefinition();
   G4Neutron::NeutronDefinition();
   G4AntiNeutron::AntiNeutronDefinition();
}

void RichTbPhysicsList::ConstructProcess()
{
 // juste OP()
//  AddTransportation();
//  ConstructGeneral();
//  ConstructEM();
  ConstructOp();
}

#include "G4Decay.hh"

void RichTbPhysicsList::ConstructGeneral()
{
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager->AddDiscreteProcess(theDecayProcess);
    }
  }
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
//#include "HpdSiEnergyLoss.hh"

void RichTbPhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  G4cout<<" Now creating EM processes"<<G4endl;
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
//    HpdSiEnergyLoss* HpdSiEnergyLossProcess =
//
//      new HpdSiEnergyLoss("Silicon","HpdSiEnergyLoss");
//    pmanager->AddProcess( HpdSiEnergyLossProcess ,-1,2,2);


    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
    //electron
      //Construct processes for electron
      pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4eIonisation(),-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),-1,-1,3);

    } else if (particleName == "e+") {
    //positron
      // Construct processes for positron
      pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4eIonisation(),-1,2,2);
       pmanager->AddProcess(new G4eBremsstrahlung(),-1,-1,3);
       pmanager->AddProcess(new G4eplusAnnihilation(),0,-1,4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon
     // Construct processes for muon
      pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4MuIonisation(),-1,2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
      pmanager->AddProcess(new G4MuPairProduction(),-1,-1,4);

    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino")) {
     // all others charged particles except geantino
	pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
	pmanager->AddProcess(new G4hIonisation(),-1,2,2);
     }
    }
  }
}

#include "G4Cerenkov.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
//#include "PadHpdPhotoElectricEffect.hh"
//#include "RichTbMaterialParameters.hh"

void RichTbPhysicsList::ConstructOp()
{
  G4cout<<"Now creating Optical processes"<<G4endl;
  G4Cerenkov*   theCerenkovProcess = new G4Cerenkov("Cerenkov");
  G4OpAbsorption* theAbsorptionProcess = new G4OpAbsorption();
  G4cout<<"Now creating the Rayleigh scattering process "<<G4endl;
  G4OpRayleigh*   theRayleighScatteringProcess = new G4OpRayleigh();
  G4cout<<"Now creating the boundary process "<<G4endl;
  G4OpBoundaryProcess* theBoundaryProcess = new G4OpBoundaryProcess();

//  PadHpdPhotoElectricEffect* theHpdPhotoElectricProcess=
//    new PadHpdPhotoElectricEffect("PadHpdPhot");

  theCerenkovProcess->SetVerboseLevel(0);
  theAbsorptionProcess->SetVerboseLevel(0);
  theRayleighScatteringProcess->SetVerboseLevel(0);
  theBoundaryProcess->SetVerboseLevel(0);

   G4int MaxNumPhotons = 300;

  theCerenkovProcess->SetTrackSecondariesFirst(true);
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);

  G4OpticalSurfaceModel themodel = unified;
  theBoundaryProcess->SetModel(themodel);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddContinuousProcess(theCerenkovProcess);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
 //     pmanager->AddDiscreteProcess(theHpdPhotoElectricProcess);
    }
  }
}

void RichTbPhysicsList::SetCuts()
{
  if (verboseLevel >1){
    G4cout << "RichTbPhysicsList::SetCuts:";
  }  
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
   SetCutsWithDefault();   
}






