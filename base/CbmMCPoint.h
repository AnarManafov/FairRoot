// -------------------------------------------------------------------------
// -----                      CbmMCPoint header file                   -----
// -----                  Created 26/07/04  by V. Friese               -----
// -------------------------------------------------------------------------


/**  CbmMCPoint.h
 *@author V.Friese <v.friese@gsi.de>
 *
 * This is the base class for all MC Points generated by the transport of
 * tracks through active detectors. All variables are stored at the
 * entrance of the track in the active volume. 
 **/



#ifndef CBMMCPOINT_H
#define CBMMCPOINT_H

#include "TObject.h"
#include "TVector3.h"

class CbmMCPoint : public TObject
{

 public:    

  /** Default constructor **/
  CbmMCPoint();


  /** Constructor with arguments
   *@param trackID  Index of MCTrack
   *@param detID    Detector ID
   *@param pos      Point coordinates [cm]
   *@param mom      Momentum of track at MCPoint [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [GeV]
   **/
  CbmMCPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom,
	     Double_t tof, Double_t length, Double_t eLoss);


  /** Destructor **/
  virtual ~CbmMCPoint();


  /** Accessors **/
  Int_t    GetTrackID()    const { return fTrackID; };
  Int_t    GetDetectorID() const { return fDetectorID; };
  Double_t GetX()          const { return fX; };
  Double_t GetY()          const { return fY; };
  Double_t GetZ()          const { return fZ; };
  Double_t GetPx()         const { return fPx; };
  Double_t GetPy()         const { return fPy; };
  Double_t GetPz()         const { return fPz; };
  Double_t GetTime()       const { return fTime; };
  Double_t GetLength()     const { return fLength; };
  Double_t GetEnergyLoss() const { return fELoss; };
  void Position(TVector3& pos) { pos.SetXYZ(fX, fY, fZ); };
  void Momentum(TVector3& mom) { mom.SetXYZ(fPx, fPy, fPz); };


  /** Modifiers **/
  void SetTrackID(Int_t id)          { fTrackID = id; };
  void SetDetectorID(Int_t id)       { fDetectorID = id; };
  void SetTime(Double_t time)        { fTime = time; };
  void SetLength(Double_t length)    { fLength = length; };
  void SetEnergyLoss(Double_t eLoss) { fELoss = eLoss; };
  void SetPosition(const TVector3& pos);
  void SetMomentum(const TVector3& mom);


  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const = 0;
  

		   
 protected:

  Int_t fTrackID;               // Track index
  Int_t fDetectorID;            // Detector ID
  Double32_t fX, fY, fZ;        // Point coordinates [cm]
  Double32_t fPx, fPy, fPz;     // Momentum components [GeV]
  Double32_t fTime;             // Time since event start [ns]
  Double32_t fLength;           // Track length since creation [cm]
  Double32_t fELoss;            // Energy loss at this point [GeV]



  ClassDef(CbmMCPoint,1)

};



inline void CbmMCPoint::SetPosition(const TVector3& pos) {
  fX = pos.X();
  fY = pos.Y();
  fZ = pos.Z();
}


inline void CbmMCPoint::SetMomentum(const TVector3& mom) {
  fPx = mom.Px();
  fPy = mom.Py();
  fPz = mom.Pz();
}


#endif
