// -------------------------------------------------------------------------
// -----                CbmUrqmdGenerator header file                  -----
// -----          Created 11/06/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------


/** CbmUrqmdGenerator.h
 *@ author V.Friese <v.friese@gsi.de>
 *@author D.Bertini <d.bertini@gsi.de>
 *
 The CbmUrqmdGenerator reads the output file 14 (ftn14) from UrQMD. The UrQMD 
 calculation has to be performed in the CM system of the collision; Lorentz
 transformation into the lab is performed by this class.
 Derived from CbmGenerator.
**/



#ifndef CBMURQMDGENERATOR_H
#define CBMURQMDGENERATOR_H


#include "CbmGenerator.h"

#include <fstream>
#include <map>
 
class TVirtualMCStack;
class CbmPrimaryGenerator;


class CbmUrqmdGenerator : public CbmGenerator
{

 public:

  /** Default constructor without arguments should not be used. **/
  CbmUrqmdGenerator();


  /** Standard constructor.
   * @param fileName The input file name
   **/
  CbmUrqmdGenerator(const char* fileName);


  /** Destructor. **/
  ~CbmUrqmdGenerator();


  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
   ** @param pStack    pointer to the stack
   ** @param ver       not used
   **/
  Bool_t ReadEvent(CbmPrimaryGenerator* primGen);

  //Skip some events in file
  Bool_t SkipEvents(Int_t count);
   
 private:

  FILE* fInputFile;                     //!  Input file

  std::map<Int_t,Int_t> fParticleTable;      //!  Map from UrQMD PID to PDGPID

  const Char_t * fFileName;             //!  Input file name

  /** Private method ReadConversionTable. Reads the conversion table
      from UrQMD particle code to PDG particle code and fills the
      conversion map. Is called from the constructor. **/
  void ReadConversionTable();

  ClassDef(CbmUrqmdGenerator,1);
 
};

#endif
   

