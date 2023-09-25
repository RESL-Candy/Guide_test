#include "SteppingAction.hh"

SteppingAction::SteppingAction(EventAction * eventAction): fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    if (!fPMTScoringVolume){
    const MyDetectorConstruction* detConstruction
        = static_cast<const MyDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fPMTScoringVolume = detConstruction->GetPMTVolume();
    }

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton")
    {
      if(aStep->GetTrack()->GetCurrentStepNumber() == 1)
        {
          fEventAction->AddCount_WholePhoton(1);
        } 
    }

    G4LogicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    if (volume != fPMTScoringVolume)
    {
        return;
    } 

    G4int copyNo = 0;

    if (volume == fPMTScoringVolume)
    {
        copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();  
        fEventAction->AddCount_PMT(fEventAction->f_PMT_Count, copyNo);
        fEventAction->AddCount_WholePMT(1);
        aStep->GetTrack()->SetTrackStatus(fStopAndKill); 
    }
}










