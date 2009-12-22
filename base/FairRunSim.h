#ifndef FAIRRUNSIM_H
#define FAIRRUNSIM_H

#include "FairRun.h"

#include "FairMCApplication.h"
#include "FairIon.h"
#include "FairParticle.h"
#include "TObjArray.h"
#include "TString.h"
#include <iostream>

class FairModule;
class FairField;
class FairPrimaryGenerator;

/**
 * Configure the Simulation session
 * @author M. Al-Turany  D. Bertini
 * @version 0.1
 * @since 12.01.04
 */
class FairRunSim : public FairRun {
public:
    /** default ctor*/
    FairRunSim();
    /** default dtor*/
    virtual ~FairRunSim();
    /** Singelton instance*/
    static FairRunSim *Instance(); 
    /**
     *       Add a module to the simulation (e.g. PIPE, Magnet, ..etc)
    */
    void        AddModule  (FairModule *Mod);
    /**
     *       Add a user defined ion to the simulation
    */
    void        AddNewIon(FairIon *ion){fIons->Add(ion);}
    /**
     *       Add a user defined ion to the simulation
    */
    void        AddNewParticle(FairParticle *Particle){fParticles->Add(Particle);}
    /**
     *       this method is used by the FAIRMCApplication
    */
    TObjArray*  GetUserDefIons();
    /**
     *       this method is used by the FAIRMCApplication
    */
    TObjArray*  GetUserDefParticles();

    /**
    *       Initialize the Simulation
    */
    virtual void        Init();
    /**
    *       run the  simulation
    */
    virtual void    Run(Int_t NEvents =0, Int_t NotUsed=0);
    /**  
    *       Set the magnetic that has to be used for simulation field
    */
    void        SetField(FairField *field);
    /**
     *       Set the event generator that has to be used for simulation field
    */
    void        SetGenerator(FairPrimaryGenerator *Gen);
    
	/** Set the material file name to be used */
    void    SetMaterials(const char * MatFileName);
    
	/**switch On/Off the track visualisation */    
    void SetStoreTraj(Bool_t storeTraj=kTRUE) {fStoreTraj = storeTraj;}
    
	/**switch On/Off the debug mode */    
    void SetTrackingDebugMode( Bool_t set ) { if (fApp) fApp->SetTrackingDebugMode( set ); }
    
	/**Set geometry builder*/
    void SetGeoModel( char * name );
    
	/**return the geometry loader used in this session*/
    TString* GetGeoModel () { return fLoaderName; }
    
	/**Get the field used in simulation*/
    FairField*  GetField() { return fField;}
    
	/**return the full list of modules used in simulation*/
    TObjArray*        GetListOfModules() { return ListOfModules;}
    
	/**Get the used primary generator*/
    FairPrimaryGenerator* GetPrimaryGenerator() { return fGen;}
    
	/**switch On/Off external decayer (Pythia) */    
    void SetPythiaDecayer(Bool_t decayer){fPythiaDecayer = decayer;}
	
	/**switch On external decayer (Pythia). Config macro will be used */    
    void SetPythiaDecayer(const TString & Config );

    /**switch On user defined decay, Config  macro will be called  */    
    void SetUserDecay(const TString& Config);

    /**switch On/Off user defined decay if true gconfig/UserDecay.C macro will be called  */    
    void SetUserDecay(Bool_t decay){fUserDecay = decay;}

	/**Flag for external decayer*/
    Bool_t  IsExtDecayer(){return fPythiaDecayer; }
    
	/**Flag for User decay*/
    Bool_t  IsUserDecay(){return fUserDecay; }
    
	/**Switch on/off Radiation length register */
    void SetRadLenRegister(Bool_t value) {fRadLength= value;}
	
	void SetUserConfig(const TString& Config) {fUserConfig = Config;}
	
	void SetUserCuts(const TString& Cuts) {fUserCuts= Cuts;}
	
	/** Set Beam energy in GeV/c */
	void SetBeamMom(Double_t BeamMom) {  fBeamMom= BeamMom; fUseBeamMom=kTRUE;}
	
	/** Get the Beam energy */
	Double_t GetBeamMom() {return fBeamMom;}
	
        /**Get beam energy flag */
        Bool_t UseBeamMom() {return fUseBeamMom;}
	void SetFieldContainer();
private:
    FairRunSim(const FairRunSim &M);
    FairRunSim& operator= (const  FairRunSim&) {return *this;}
	void SetMCConfig();
	void CheckFlukaExec();
protected:

    Int_t                  count;//!		                       /** Internal counter*/
    FairMCApplication*     fApp;  //!		                       /** Main VMC application */
    Double_t               fBeamMom; //!	                       /** Beam Energy in GeV/c  */
    Bool_t                 fUseBeamMom; //!	                       /** flag for use Beam Energy  */
    FairPrimaryGenerator*  fGen; //!                               /** Primary Event Generator */
    static FairRunSim*     fginstance;//!                          /** Singelton Instance */ 
    FairField*             fField;                                 /** Magnetic Field */ 
    const char*            fMapName; //!                           /** Input file name map*/
    TObjArray*             fIons; //!                              /** Array of user defined ions */
    TObjArray*             fParticles; //!                         /** Array of user defined particles*/
    TObjArray*             ListOfModules;//!                       /** Array of used modules */
    TString 	           MatFname; //!                           /** Material file name */
    Bool_t                 fStoreTraj;   //!                       /** Trajectory store flags */
    TString*               fLoaderName;  //!                       /** Geometry Model (TGeo or G3)*/
    Bool_t                 fPythiaDecayer;  //!                    /** flag for using Pythia decayer*/
	TString                fPythiaDecayerConfig; //!               /** Macro for Pythia decay configuration*/
    Bool_t                 fUserDecay;                             /** flag for setting user decay */
    TString                fUserDecayConfig; //!                   /** Macro for decay configuration*/
    Bool_t                 fRadLength;   //!                       /** flag for registring radiation length*/
	TString                fUserConfig; //!                        /** Macro for geant configuration*/
	TString                fUserCuts; //!                          /** Macro for geant cuts*/
	
    
    ClassDef(FairRunSim ,1)
    
};

#endif //FAIRRUNSIM_H





