#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* P_generator = new PrimaryGeneratorAction();
  SetUserAction(P_generator);
  
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
  
  EventAction* eventAction = new EventAction(runAction);
  SetUserAction(eventAction);
  
  SteppingAction* StepAction = new SteppingAction(eventAction);
  SetUserAction(StepAction);
}