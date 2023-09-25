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
    // pmt 두개 이므로 pmt의 광자 수도 두 군데에서 카운트 
    for(int i=0;i<2;i++){
        f_PMT_Count[i] = {0,};
    }
}

void EventAction::EndOfEventAction(const G4Event*)
{
    fEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //0번은 runaction.cc파일의 목차에서 CreateNtupleDColumn중 첫번째에 값이 들어감
    //그 뒤로 순차적으로 넘버링 
    analysisManager->FillNtupleDColumn(0, fEventID);
    //fCount1,2, f_PMT_count에 해당하는 값들은 steppingaction.cc에서 넣어줌 
    //그 값들을 어떤 식으로 더하는지는 eventaction.hh 에서 함수로 만듦
    for(G4int i=0; i<2; i++){
        analysisManager->FillNtupleDColumn(i+1, f_PMT_Count[i]);    
        }
    analysisManager->FillNtupleDColumn(3, fCount1);
    analysisManager->FillNtupleDColumn(4, fCount2);
    analysisManager->AddNtupleRow();  
    fRunAction->printEventproc();
}

