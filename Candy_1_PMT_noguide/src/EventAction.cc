#include "EventAction.hh"

EventAction::EventAction(RunAction * runAction)
: fRunAction(runAction)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fCount1 = 0;
    fCount2 = 0;
    for(int i=0;i<2;i++){
        f_PMT_Count[i] = {0,};
    }
}

void EventAction::EndOfEventAction(const G4Event*)
{
    fEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, fEventID);
    for(G4int i=0; i<2; i++){
        analysisManager->FillNtupleDColumn(i+1, f_PMT_Count[i]);    
        }
    analysisManager->FillNtupleDColumn(3, fCount1);
    analysisManager->FillNtupleDColumn(4, fCount2);
    analysisManager->AddNtupleRow();  
    fRunAction->printEventproc();
}

