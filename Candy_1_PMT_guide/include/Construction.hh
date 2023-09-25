#ifndef Construction_h
#define Construction_h

#include "G4VUserDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"


#include "G4MaterialTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4VisAttributes.hh" 

#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction();
  ~MyDetectorConstruction() override;
  
  G4VPhysicalVolume* Construct() override;

  //다른 파일에서 pmt logical volume 가져오게 해주는 함수(?) 만들어주기 
  G4LogicalVolume* GetPMTVolume() const { return flogical_PMT; }

protected:
  //construction.cc 에서 설정 안하고 여기서 선언(빈걸로) 
  G4LogicalVolume* flogical_PMT = nullptr;

private:

};
#endif
