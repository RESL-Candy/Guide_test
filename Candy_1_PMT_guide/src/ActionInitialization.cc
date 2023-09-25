#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{

  // 뭘 쏠지 정해주기 (여기서는 큰 틀만 잡고 세부적인 것은 macro로 실행해도 됨)
  PrimaryGeneratorAction* P_generator = new PrimaryGeneratorAction();
  SetUserAction(P_generator);
  
  //한 번 프로그램 돌리면 돌아가는 전체 내용 설정  ex: 광자 100개 얍
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
  
  //내가 설정한 입자 한번에 해당하는 내용 설정 ex: 첫번째 광자에 해당  
  EventAction* eventAction = new EventAction(runAction);
  SetUserAction(eventAction);
  
  //입자가 겪는 step마다 해야할 일 설정 
  SteppingAction* StepAction = new SteppingAction(eventAction);
  SetUserAction(StepAction);
}