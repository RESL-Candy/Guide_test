#ifndef SteppingAction_h
#define SteppingAction_h 


#include "EventAction.hh"
#include "Construction.hh"


#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"


//#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4UserSteppingAction.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction* eventAction);
  ~SteppingAction() override;
  
  // method from the base class
  void UserSteppingAction(const G4Step*) override;
  
private:
  EventAction* fEventAction = nullptr;
  //빈 껍데기 만들어놓기 중요..
  G4LogicalVolume* fPMTScoringVolume = nullptr;
};

#endif