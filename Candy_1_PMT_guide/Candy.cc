#include <iostream>
#include "G4RunManager.hh"

#include "G4RandomTools.hh"
#include "Randomize.hh"

#include "Construction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4Vismanager.hh"

#include "G4SteppingVerbose.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv)// argc 에는 정보의 갯수, argv에는 입력값이 들어감
{
    long seed = 42; // Replace with your desired seed value
    CLHEP::HepRandom::setTheSeed(seed);
    // 그냥 실행하면 argc가 1로 저장됨 
    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }
  
    //use G4SteppingVerboseWithUnits
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);
  
    // Construct the default run manager
    G4RunManager * runManager = new G4RunManager();

    MyDetectorConstruction* detectorconstruction = new MyDetectorConstruction();
    runManager->SetUserInitialization(detectorconstruction);

    // Physics List
    // 어떤 물리법칙이 적용될지
    G4VModularPhysicsList* physicsList = new MyPhysicsList();
    runManager->SetUserInitialization(physicsList);

    // User action initialization
    // 다른 다양한 설정들 (초기입자, step, event, run) 등등
    ActionInitialization* actioninit = new ActionInitialization();
    runManager->SetUserInitialization(actioninit);

    G4VisManager *visManager = new G4VisExecutive("Quiet");// 쓸때없는 warning message 안뜨게 
    visManager ->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer(); 

    if ( ! ui ) 
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);    
    }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");

        G4cout<<"/////////////////////////////////////////////////////////////"<<G4endl;
        G4cout<<"//                                                         //"<<G4endl;
        G4cout<<"//                                                         //"<<G4endl;
        G4cout<<"//                                                         //"<<G4endl;
        G4cout<<"//    ///////      /////////      ///////     //           //"<<G4endl;
        G4cout<<"//    //    //     //           //     ///    //           //"<<G4endl;
        G4cout<<"//    //     //    //           //       //   //           //"<<G4endl;
        G4cout<<"//    //     //    //           ///           //           //"<<G4endl;
        G4cout<<"//    //   ///     /////////     //           //           //"<<G4endl;
        G4cout<<"//    //////       //             ////////    //           //"<<G4endl;
        G4cout<<"//    //   //      //                    //   //           //"<<G4endl;
        G4cout<<"//    //    //     //                    //   //           //"<<G4endl;
        G4cout<<"//    //     //    //           //      ///   //           //"<<G4endl;
        G4cout<<"//    //      //   /////////     /////////    ///////////  //"<<G4endl;
        G4cout<<"//                                                         //"<<G4endl;
        G4cout<<"//                                                         //"<<G4endl;
        G4cout<<"//                                                         //"<<G4endl;
        G4cout<<"/////////////////////////////////////////////////////////////"<<G4endl;
        G4cout<<"                                                             "<<G4endl;
        G4cout<<"                            HELLO                            "<<G4endl;

        ui->SessionStart();

        ///이 밑에 뭐 입력하면 오류남 조심///
 
    
    
        delete ui;
    }

    delete visManager;
    delete runManager;

    G4cout<<"/////////////////////////////////////////////////////////////"<<G4endl;
    G4cout<<"//                                                         //"<<G4endl;
    G4cout<<"//                                                         //"<<G4endl;
    G4cout<<"//                                                         //"<<G4endl;
    G4cout<<"//      //////     ///      ///   // //////   //     //    //"<<G4endl;
    G4cout<<"//    ///         ////     ////  // //    //  //    //     //"<<G4endl;
    G4cout<<"//    //         // //    // // // //     //  //  //       //"<<G4endl;
    G4cout<<"//    //        //////   //  //// //      //  ////         //"<<G4endl;
    G4cout<<"//    ///      //   //  //   /// //     //    //           //"<<G4endl;
    G4cout<<"//     ////// //    // //    // ////////     //            //"<<G4endl;
    G4cout<<"//                                                         //"<<G4endl;
    G4cout<<"//                                                         //"<<G4endl;
    G4cout<<"//                                                         //"<<G4endl;
    G4cout<<"/////////////////////////////////////////////////////////////"<<G4endl;
    G4cout<<"                                                             "<<G4endl;
    G4cout<<"                         Good bye :)                         "<<G4endl;


    return 0;
}
