#include "PhysicsList.hh"


  
MyPhysicsList::MyPhysicsList()
{
  new FTFP_BERT; // 기본 physics 패키지 
  RegisterPhysics (new G4EmStandardPhysics()); // 기본 (옵션도 추가 가능)
  RegisterPhysics (new G4OpticalPhysics()); // 기본 (")
  //특별한 physics가 있을 경우 register로 추가 등록 
}

MyPhysicsList::~MyPhysicsList()
{
}

// 이 밑에 constructprocess , constructparticle 과 같은 식으로 
// 하나하나 physicslist를 만들어줄 수도 있음 