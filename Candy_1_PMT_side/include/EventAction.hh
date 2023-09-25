#ifndef CANDYEventAction_h
#define CANDYEventAction_h 1

#include "G4UserEventAction.hh"


#include "G4AnalysisManager.hh"


#include "RunAction.hh"
#include "G4UserRunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    ~EventAction() override;
  
    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;
  
    void AddCount_WholePhoton(G4int Count1) { fCount1 += Count1; }
    void AddCount_WholePMT(G4int Count2) {fCount2 += Count2;}
    void AddCount_PMT(G4int N[2], G4int P)
    {
        N[P] += 1;
    }
    G4int f_PMT_Count[2] = {0,};

private:
    RunAction* fRunAction = nullptr;
    G4int   fCount1 = 0;    
    G4int   fCount2 = 0;
    G4int   fEventID=0;
};

#endif