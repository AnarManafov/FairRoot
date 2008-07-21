// Class for the representation of a track as helix (SC system)
//
// Authors: M. Al-Turany, A. Fontana, L. Lavezzi and A. Rotondi
//
//
// GEANE parameters (q/p, lambda, phi, y_perp, z_perp) of Helix track 
// The Helix can be constructed using the Helix parameter (1/p, lambda, phi,y_perp,z_perp) in SC reference 
// and the covariance matrix. Or using position and momentum in LAB referance. 


#include "CbmTrackParH.h"

#include "CbmGeaneUtil.h"
#include "CbmRunAna.h"
#include "CbmField.h"

#include "TMath.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;   

ClassImp(CbmTrackParH)

CbmTrackParH::CbmTrackParH(): CbmTrackPar() {
  
    for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=0;
  }

}
// constructor in SC
CbmTrackParH::CbmTrackParH(Double_t x, Double_t y, Double_t z,
			   Double_t lm, Double_t phi, Double_t qp,
			   Double_t CovMatrix[15])
  : CbmTrackPar()
{
 
  fLm = lm;
  fPhi = phi;
  fQp = qp;
  Double_t P  = TMath::Abs(1/fQp);
  fq= P * fQp;
  for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=CovMatrix[i];
  }

  cLm= TMath::Cos(fLm);
  sLm= TMath::Sin(fLm);
  cphi= TMath::Cos(fPhi);
  sphi= TMath::Sin(fPhi);

  fPx = P * cLm* cphi;
  fPy = P * cLm* sphi;
  fPz = P * sLm;


  fX_sc   = x;
  fY_sc   = y;
  fZ_sc   = z;

  fX =fX_sc*cLm*cphi -fY_sc*sphi -fZ_sc*cphi*sLm;
  fY =fX_sc*cLm*sphi +fY_sc*cphi -fZ_sc*sphi*sLm;
  fZ =fX_sc*sLm +fZ_sc*cLm;

  fDQp  = TMath::Sqrt(fabs(fCovMatrix[0]));
  fDLm  = TMath::Sqrt(fabs(fCovMatrix[5]));
  fDPhi = TMath::Sqrt(fabs(fCovMatrix[9]));
  fDX_sc   = 0;
  fDY_sc   = TMath::Sqrt(fabs(fCovMatrix[12]));
  fDZ_sc   = TMath::Sqrt(fabs(fCovMatrix[14]));

}

//constructor in LAB
CbmTrackParH::CbmTrackParH(TVector3 pos, TVector3 Mom, TVector3 posErr, TVector3 MomErr, Double_t q)
  : CbmTrackPar(pos.x(),pos.y(),pos.z(),Mom.x(),Mom.y(),Mom.z(),q)
{
  Reset();
  SetPx(Mom.x());
  SetPy(Mom.y());
  SetPz(Mom.z());

  SetX(pos.x()); //x (lab)
  SetY(pos.y()); //y (lab)
  SetZ(pos.z()); //z (lab)


 
  fq= q/TMath::Abs(q);
  Double_t P =TMath::Sqrt(fPx*fPx+fPy*fPy+fPz*fPz);
  fLm    = TMath::ASin(fPz/P);

  fPhi   = TMath::ATan2(fPy,fPx);
  fQp    = fq/P;
  
  cLm= TMath::Cos(fLm);                                       
  sLm= TMath::Sin(fLm);
  cphi= TMath::Cos(fPhi);              
  sphi= TMath::Sin(fPhi);

  fDPx= MomErr.x(); //dpx
  fDPy= MomErr.y(); //dpy
  fDPz= MomErr.z(); //dpz
  
  fX_sc   = fX*cphi*cLm+ fY*cLm*sphi+fZ*sLm;
  fY_sc   = fY*cphi-fX*sphi; 
  fZ_sc   = fZ*cLm-fY*sLm*sphi-fX*sLm*cphi;
 
  fDX   = posErr.x(); //dx (lab)
  fDY   = posErr.y(); //dy (lab)
  fDZ   = posErr.z(); //dz (lab)

  

  Double_t PD[3],RD[6][6],H[3],CH,PC[3],RC[15];
  
  PD[0]=Mom.x();
  PD[1]=Mom.y();
  PD[2]=Mom.z();
  for(Int_t i=0;i<6;i++)  for(Int_t j=0;j<6;j++)  {
	RD[i][j]=0;
  }
  RD[0][0] = fDPx * fDPx;  
  RD[1][1] = fDPy * fDPy;  
  RD[2][2] = fDPz * fDPz; 
  RD[3][3] = fDX * fDX;  
  RD[4][4] = fDY * fDY;  
  RD[5][5] = fDZ * fDZ;  

  // retrieve field
  Double_t pnt[3];
  pnt[0] = fX; 
  pnt[1] = fY;
  pnt[2] = fZ;
  CbmRunAna *fRun = CbmRunAna::Instance();
  fRun->GetField()->GetFieldValue(pnt, H);

  CH=fq;
  CbmGeaneUtil util;
  
  util.FromMarsToSC(PD,RD,H,CH,PC,RC);
 
  for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=RC[i];
  }

  fDX_sc   = 0;
  fDY_sc   = TMath::Sqrt(fabs(fCovMatrix[12]));
  fDZ_sc   = TMath::Sqrt(fabs(fCovMatrix[14]));
 
}
// track definition in LAB
void CbmTrackParH::SetTrackPar(Double_t X,  Double_t Y,  Double_t Z,
		Double_t Px, Double_t Py, Double_t Pz, Double_t Q,
		Double_t  CovMatrix[15])
{
  Reset();
  Double_t P =TMath::Sqrt(Px*Px+Py*Py+Pz*Pz);
  fLm    = TMath::ASin(Pz/P);
 
    fPhi   = TMath::ATan2(Py,Px);
  if (Q!=0) fq = TMath::Abs(Q)/Q;
  fQp    = fq/P;
  
  cLm= TMath::Cos(fLm);
  sLm= TMath::Sin(fLm);
  cphi= TMath::Cos(fPhi);
  sphi= TMath::Sin(fPhi);

  SetX(X);  
  SetY(Y);
  SetZ(Z);

  SetPx(Px);
  SetPy(Py);
  SetPz(Pz);

  fX_sc   = fX*cphi*cLm+ fY*cLm*sphi+fZ*sLm;
  fY_sc   = fY*cphi-fX*sphi; 
  fZ_sc   = fZ*cLm-fY*sLm*sphi-fX*sLm*cphi;
      
  for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=CovMatrix[i];
  }

  fDQp  = TMath::Sqrt(fabs(fCovMatrix[0]));
  fDLm  = TMath::Sqrt(fabs(fCovMatrix[5]));
  fDPhi = TMath::Sqrt(fabs(fCovMatrix[9]));
  fDX_sc   = 0;
  fDY_sc   = TMath::Sqrt(fabs(fCovMatrix[12]));
  fDZ_sc   = TMath::Sqrt(fabs(fCovMatrix[14]));
 
  fDPx= GetDPx(); 
  fDPy= GetDPy(); 
  fDPz= GetDPz(); 

  
  Double_t PD[3],RD[6][6],H[3],CH,PC[3],RC[15];

  PC[0]   = fQp;
  PC[1]   = fLm; 
  PC[2]   = fPhi;
  
  for(Int_t i=0;i<15;i++)  {
	RC[i]=fCovMatrix[i];
  }

  // retrieve field
  Double_t pnt[3];
  pnt[0] = fX; 
  pnt[1] = fY;
  pnt[2] = fZ;
  CbmRunAna *fRun = CbmRunAna::Instance();
  fRun->GetField()->GetFieldValue(pnt, H);

  CH=fq;
  CbmGeaneUtil util;
  util.FromSCToMars(PC,RC,H,CH,PD,RD);
  
  fDPx = sqrt(fabs(RD[0][0]));
  fDPy = sqrt(fabs(RD[1][1]));
  fDPz = sqrt(fabs(RD[2][2]));
  fDX = sqrt(fabs(RD[3][3]));
  fDY = sqrt(fabs(RD[4][4]));
  fDZ = sqrt(fabs(RD[5][5]));
 
 
 }

//track definition in SC
void  CbmTrackParH::SetTrackPar(Double_t x,  Double_t y,  Double_t z,
		 Double_t qp, Double_t lm, Double_t phi, Double_t CovMatrix[15])
{
  fLm = lm;
  fPhi = phi;
  fQp = qp;
  Double_t P  = TMath::Abs(1/fQp);
  fq= P * fQp;
  for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=CovMatrix[i];
        
  }

  cLm= TMath::Cos(fLm);
  sLm= TMath::Sin(fLm);
  cphi= TMath::Cos(fPhi);
  sphi= TMath::Sin(fPhi);

  fPx = P * cLm* cphi;
  fPy = P * cLm* sphi;
  fPz = P * sLm;

  fX_sc   = x;
  fY_sc   = y;
  fZ_sc   = z;

  fX =fX_sc*cLm*cphi -fY_sc*sphi -fZ_sc*cphi*sLm;
  fY =fX_sc*cLm*sphi +fY_sc*cphi -fZ_sc*sphi*sLm;
  fZ =fX_sc*sLm +fZ_sc*cLm;

  fDQp  = TMath::Sqrt(fabs(fCovMatrix[0]));
  fDLm  = TMath::Sqrt(fabs(fCovMatrix[5]));
  fDPhi = TMath::Sqrt(fabs(fCovMatrix[9]));
  fDX_sc   = 0;
  fDY_sc   = TMath::Sqrt(fabs(fCovMatrix[12]));
  fDZ_sc   = TMath::Sqrt(fabs(fCovMatrix[14]));

  fDPx= GetDPx() ; 
  fDPy= GetDPy() ; 
  fDPz= GetDPz() ; 
  

  Double_t PD[3],RD[6][6],H[3],CH,PC[3],RC[15];

  PC[0]   = fQp;
  PC[1]   = fLm; 
  PC[2]   = fPhi;
  
  for(Int_t i=0;i<15;i++)  {
	RC[i]=fCovMatrix[i];
  }

  // retrieve field
  Double_t pnt[3];
  pnt[0] = fX; 
  pnt[1] = fY;
  pnt[2] = fZ;
  CbmRunAna *fRun = CbmRunAna::Instance();
  fRun->GetField()->GetFieldValue(pnt, H);

  CH=fq;
  CbmGeaneUtil util;
  util.FromSCToMars(PC,RC,H,CH,PD,RD);

// test
  fDPx = sqrt(fabs(RD[0][0]));
  fDPy = sqrt(fabs(RD[1][1]));
  fDPz = sqrt(fabs(RD[2][2]));
   fDX = sqrt(fabs(RD[3][3]));
   fDY = sqrt(fabs(RD[4][4]));
   fDZ = sqrt(fabs(RD[5][5]));
}


void CbmTrackParH::CalCov()
{

  fDPx= GetDPx(); 
  fDPy= GetDPy(); 
  fDPz= GetDPz(); 

  Double_t fP =sqrt(fPx*fPx+fPy*fPy+fPz*fPz);
  Double_t fDP= (2*fPx*fDPx+2*fPy*fDPy+2*fPz*fDPz)/(2*fP); //dp 
  fDQp = - fDP/(fP*fP);  // d(1/p)
  
  Double_t fDPQDLm = - sLm * cphi / fPx; // d(1/p)/dLambda
  Double_t fDPQDPhi= - cLm * TMath::Sin(fPhi) / fPx; // d(1/p)/dphi
  
  Double_t fDPQDX_sc  = (fX_sc*pow(fZ,3))/(fPz*pow(pow(fX_sc,2) + pow(fY_sc,2),2.5)*
                     pow(1 + pow(fZ_sc,2)/(pow(fX_sc,2) + pow(fY_sc,2)),1.5)) -
		     (fX_sc*fZ_sc)/(fPz*pow(pow(fX_sc,2) + 
                     pow(fY_sc,2),1.5)*TMath::Sqrt(1 + pow(fZ_sc,2)/(pow(fX_sc,2) + pow(fY_sc,2))));  //d(1/p)/dx

  Double_t fDPQDY_sc  = (fY_sc*pow(fZ_sc,3))/(fPz*pow(pow(fX_sc,2) + pow(fY_sc,2),2.5)*
                     pow(1 + pow(fZ_sc,2)/(pow(fX_sc,2) + pow(fY_sc,2)),1.5)) -
                     (fY_sc*fZ_sc)/(fPz*pow(pow(fX_sc,2) + pow(fY_sc,2),1.5)*
                     TMath::Sqrt(1 + pow(fZ_sc,2)/(pow(fX_sc,2) + pow(fY_sc,2))));                  //d(1/p)/dy

  fDLm= (-((fPz*fDP)/pow(fP,2)) + fDPz/fP)/ TMath::Sqrt(1 - pow(fPz,2)/pow(fP,2)); // d(Lambda)
  
  Double_t fDLmDPhi =-((fPx*TMath::Tan(fPhi)/cphi)/
      (fP*TMath::Sqrt(1 - (pow(fPx,2)*pow(1/cphi,2))/pow(fP,2)))); // dLambda/dPhi

  Double_t fDLmDX_sc = -((fX_sc*fZ_sc)/(pow(pow(fX_sc,2) + pow(fY_sc,2),1.5)*
        (1 + pow(fZ_sc,2)/(pow(fX_sc,2) + pow(fY_sc,2)))));                // dLambda/dX

  Double_t fDLmDY_sc = -((fY_sc*fZ_sc)/(pow(pow(fX_sc,2) + pow(fY_sc,2),1.5)*
        (1 + pow(fZ_sc,2)/(pow(fX_sc,2) + pow(fY_sc,2)))));               // dLambda/dY
  
  fDPhi=   (-((fPy*fDP/cLm)/pow(fP,2)) + 
	    (fDPy/cLm)/fP +(fPy*fDLm*TMath::Tan(fLm)/cLm)/fP) /TMath::Sqrt(1 - (pow(fPy,2)*pow(1/cLm,2))/pow(fP,2));  // d(Phi)
  
  Double_t fDPhiDx_sc = -(fY_sc/(pow(fX_sc,2)*(1 + pow(fY_sc,2)/pow(fX_sc,2))));  //   dPhi/dx
 
  Double_t fDPhiDy_sc = 1/(fX_sc*(1 + pow(fY_sc,2)/pow(fX_sc,2))); //    dPhi/dy

  Double_t fDXDY_sc = 1/TMath::Tan(fPhi);                              // dX/dY
  
  if(fDQp==0 && fDLm==0 && fDPhi ==0 ){
    cout << "CbmTrackParH::CalCov()  No Errors" << endl;
  }else{
	fCovMatrix[0]  =fDQp*fDQp; 
	fCovMatrix[1]  =fDPQDLm;
	fCovMatrix[2]  =fDPQDPhi;
	fCovMatrix[3]  =fDPQDX_sc;
	fCovMatrix[4]  =fDPQDY_sc;
	fCovMatrix[5]  =fDLm*fDLm;
	fCovMatrix[6]  =fDLmDPhi;
	fCovMatrix[7]  =fDLmDX_sc;
	fCovMatrix[8]  =fDLmDY_sc;
	fCovMatrix[9]  =fDPhi*fDPhi;
	fCovMatrix[10] =fDPhiDx_sc;
	fCovMatrix[11] =fDPhiDy_sc;
	fCovMatrix[12] =fDX_sc*fDX_sc;
	fCovMatrix[13] =fDXDY_sc;
	fCovMatrix[14] =fDY_sc*fDY_sc;
  }
}

CbmTrackParH::CbmTrackParH(CbmTrackPar &Trkbase)
{
     cout << "CbmTrackParH::CbmTrackParH(CbmTrackPar &Trkbase)"     << endl;
  Reset();
  fX = Trkbase.GetX();
  fY = Trkbase.GetY();
  fZ = Trkbase.GetZ();
  
  fPx = Trkbase.GetPx();
  fPy = Trkbase.GetPy();
  fPz = Trkbase.GetPz();
  fq = Trkbase.GetQ();
  Double_t fP=TMath::Sqrt(fPx*fPx +fPy*fPy +fPz*fPz );
  if (fP!=0) fQp = fq/fP;
  else fQp=0;
  fLm    = TMath::ASin(fPz/fP);
  fPhi   = TMath::ATan2(fPy,fPx);
  
  cLm= TMath::Cos(fLm);
  sLm= TMath::Sin(fLm);
  cphi= TMath::Cos(fPhi);
  sphi= TMath::Sin(fPhi);

  fDPx  = Trkbase.GetDPx();
  fDPy  = Trkbase.GetDPy();
  fDPz  = Trkbase.GetDPz();

  fDX   = Trkbase.GetDX();
  fDY   = Trkbase.GetDY();
  fDZ   = Trkbase.GetDZ();
  
  fX_sc   = fX*cphi*cLm+ fY*cLm*sphi+fZ*sLm;
  fY_sc   = fY*cphi-fX*sphi;
  fZ_sc   = fZ*cLm-fY*sLm*sphi-fX*sLm*cphi;

  for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=0;
  }

  Double_t PD[3],RD[6][6],H[3],CH,PC[3],RC[15];
  
  PD[0]=fPx;
  PD[1]=fPy;
  PD[2]=fPz;
  for(Int_t i=0;i<6;i++)  for(Int_t j=0;j<6;j++)  {
	RD[i][j]=0;
  }
  RD[0][0] = fDPx * fDPx;  
  RD[1][1] = fDPy * fDPy;  
  RD[2][2] = fDPz * fDPz;  
  RD[3][3] = fDX * fDX;
  RD[4][4] = fDY * fDY;  
  RD[5][5] = fDZ * fDZ;  

  // retrieve field
  Double_t pnt[3];
  pnt[0] = fX; 
  pnt[1] = fY;
  pnt[2] = fZ;
  CbmRunAna *fRun = CbmRunAna::Instance();
  fRun->GetField()->GetFieldValue(pnt, H);

  CH=fq;
  CbmGeaneUtil fUtil;
  fUtil.FromMarsToSC(PD,RD,H,CH,PC,RC);
  for(Int_t i=0;i<15;i++)  {
	fCovMatrix[i]=RC[i];
  }
  fDX_sc   = 0;
  fDY_sc   = TMath::Sqrt(fabs(fCovMatrix[12]));
  fDZ_sc   = TMath::Sqrt(fabs(fCovMatrix[14]));
}

CbmTrackParH::~CbmTrackParH() 
{

}

void CbmTrackParH::Print() {
  cout << this<< endl;
  cout << "Position : (";
  cout.precision(2);
  cout << fX << ", " << fY << ", " << fZ << ")" << endl;
  cout << "Angles : Lambda = " << fLm << ", Phi = " << fPhi << endl;
  cout << "q/p = " << fQp << endl;
  for(Int_t i=0;i<15;i++) {
        cout << "fCovMatrix[" << i << "] = " <<  this << " " << fCovMatrix[i] << endl;
  }

}
Double_t CbmTrackParH::GetDX_sc()   
{ 
	return fDX_sc; 
}

Double_t CbmTrackParH::GetDY_sc()   
{ 
	return fDY_sc; 
}
Double_t CbmTrackParH::GetDZ_sc()  
{ 
	return fDZ_sc; 
}

Double_t CbmTrackParH::GetX()   
{  
    if(fX==0){
      fX =fX_sc*cLm*cphi -fY_sc*sphi -fZ_sc*cphi*sLm;
    }
    return fX;
}
Double_t CbmTrackParH::GetY()   
{  
    if(fY==0){
      fY =fX_sc*cLm*sphi +fY_sc*cphi -fZ_sc*sphi*sLm;
    }
    return fY;
}

Double_t CbmTrackParH::GetZ()   
{  
   if(fZ==0){ 
      fZ =fX_sc*sLm +fZ_sc*cLm;
   }
   return fZ;
}


Double_t CbmTrackParH::GetDPx()  
{ 

  return fDPx;
}
Double_t CbmTrackParH::GetDPy()  
{
   
  return fDPy;
}
Double_t CbmTrackParH::GetDPz()  
{ 	

  
  return fDPz;
}

Double_t CbmTrackParH::GetDQp()  
{ 
	return fDQp;
}
Double_t CbmTrackParH::GetDLambda() 
{ 	return fDLm; 
};
Double_t CbmTrackParH::GetDPhi()  
{ 
	return fDPhi; 
};

void  CbmTrackParH::Reset()
{
   fLm=0; 
   fPhi=0;
   fDLm=0; 
   fDPhi=0;
   fX_sc= fY_sc= fZ_sc=0;
   fDX_sc= fDY_sc= fDZ_sc =0;
   cLm =sLm= cphi =sphi=0; 
   for(Int_t i= 0;i<15;i++)  { fCovMatrix[i] =0; }

   //base class members

   fX= fY= fZ=0;
   fDX= fDY= fDZ=0;
   fPx= fPy= fPz=0;
   fDPx= fDPy= fDPz=0;
   fQp=fDQp=fq=0;
 

}

void CbmTrackParH::GetCovQ(Double_t *CovQ){
  // return error matrix in 1/p instead of q/p

  for(int i = 0; i < 15; i++){
    CovQ[i] = fCovMatrix[i];
    if(fq!=0)
      {
	if(i == 0) CovQ[i] = CovQ[i] / (fq * fq);
	if(i > 0 && i < 5) CovQ[i] = CovQ[i] / fq;
      }
  }
}



