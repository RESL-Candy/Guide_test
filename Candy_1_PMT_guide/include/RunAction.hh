#ifndef RunAction_h
#define RunAction_h 


#include "PrimaryGeneratorAction.hh"
#include "Construction.hh"


#include "G4UserRunAction.hh"

#include "G4AnalysisManager.hh" //for make root file

#include "G4RunManager.hh"
#include "G4Run.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction() override;
  
  void BeginOfRunAction(const G4Run*) override;
  void   EndOfRunAction(const G4Run*) override;
  void printEventproc();


private:
  // 여기는 빈 껍데기를 만드는 곳이므로 여기 수치는 바꿀 필요 없음
  G4int fEvent =0;
  G4int nofEvents=0;  
};


#endif
