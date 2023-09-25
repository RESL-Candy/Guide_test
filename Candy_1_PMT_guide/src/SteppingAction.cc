#include "SteppingAction.hh"

SteppingAction::SteppingAction(EventAction * eventAction): fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  //처음에 PMT scoringvolume 설정 (내 detection 영역은 이거다 라는 뜻) 
  if (!fPMTScoringVolume){
    const MyDetectorConstruction* detConstruction
        = static_cast<const MyDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fPMTScoringVolume = detConstruction->GetPMTVolume();
    }

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //어떤 스텝에서 그 스텝에 해당하는 입자가 광자 라면 
  if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton")
    {
      //그 스텝이 해당 트랙의 맨 첫번째 스텝이라면 (입자의 첫번째 스텝이라는 뜻)
      if(aStep->GetTrack()->GetCurrentStepNumber() == 1)
        {
          //eventaction.hh에서 정한 AddCount_WholePhton이라는 함수에 1을 넣어라.
          fEventAction->AddCount_WholePhoton(1);
        } 
    }
    //현재 스텝이 위치한 volume 불러오기 
    G4LogicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    if (volume != fPMTScoringVolume)
    {
        return;
    } 

    G4int copyNo = 0;
    //만약 지금 pmt에 위치해있다면 
    if (volume == fPMTScoringVolume)
    {
        //copynumber을 불러옴 (pmt 두 개 중 어디인지 알기 위해 0 or 1)
        copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();  
        fEventAction->AddCount_PMT(fEventAction->f_PMT_Count, copyNo);
        //pmt 양쪽 다 합쳐서 있는 것을 바로 보는 것이 시간절약 상 좋아서.. 추가.. 
        fEventAction->AddCount_WholePMT(1);
        //이렇게 pmt에서 detect 되면 더 이상 track을 나아가지 않고 (stop) 죽이기 (kill) 
        aStep->GetTrack()->SetTrackStatus(fStopAndKill); 
    }
}










