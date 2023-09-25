#include "PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Navigator.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

//macro 파일로 설정하기 위해 이 파일은 최대한 덜어냄.  
//macro 파일 원하는 방향으로 설정 실패..
//general particle source를 사용하지만 이전 코드와의 편의성을 위해 
//fparticlegun으로 명명

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),fParticleGun(0)
{
  fParticleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);  
}

// 아래 부분으로 변경하면 macro 없이 설정
// water volume 내부의 random한 지점에서 random한 방향으로 광자 발생 

/* 
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
  G4int nofParticles = 1;            //한번에 몇개씩 쏠지 결정 (1000개로 할 경우 동일한 지점에서 1000개 생성)
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic

  G4String particle_name = "opticalphoton";     //광자로 쏘겠다고 결정 ex: gamma, electron 등등 
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(particle_name);

  fParticleGun->SetParticleDefinition(particleDefinition);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //새로운 변수에 logical water volume 불러와서 넣어주기
  G4LogicalVolume * logicalwater = G4LogicalVolumeStore::GetInstance()->GetVolume("logical_water");
   
  //안됐을 때 에러 띄우고 끄기 
  if (!logicalwater) 
  {
    G4cerr << "Logical volume 'logical_water' not found. Check your geometry setup." << G4endl;
    return;
  }

  //navigator: 현 위치 잡아주는 친구인듯 , navigator 부분은 포럼 긁어온 느낌이라 이해 완x

  G4Navigator* navigator = new G4Navigator();

  navigator->SetWorldVolume(G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume());
  
  G4ThreeVector randomPosition;
  while (true) 
  {
    //water의 surface * (0~1) 은 내부 임의의 한 점이라고 생각하여 설정
    //이 외에 애초에 contruction에서 지정한 volume 내의 한 점으로 설정하는 방식은 없는 것으로 보임  

    randomPosition = G4UniformRand() * (logicalwater->GetSolid()->GetPointOnSurface());

    // navigator가 내부의 randompoint에 있어야 돌아감 (인듯?)
    if (navigator->LocateGlobalPointAndSetup(randomPosition)){
      break;
    }
  } 
  //입자 생성 위치 설정
  fParticleGun->SetParticlePosition(randomPosition);

  G4double energy = 3.0*eV; // 대강 가시광선의 에너지 영역대 (푸른빛)
  G4double phi = G4UniformRand() * 2.0 * CLHEP::pi;
  G4double Theta = G4UniformRand() * 2.0 * CLHEP::pi;  // random angle
  
  //반지름이 1 짜리인 구의 임의의 한 점 (momentum 표시용)
  G4double x_m = G4UniformRand()* std::cos(phi)* std::cos(Theta); // Random x between -1 and 1
  G4double y_m = G4UniformRand()* std::sin(phi)* std::cos(Theta); // Random y between -1 and 1
  G4double z_m = G4UniformRand()* std::sin(Theta); // Random z between -1 and 1

  //입자 에너지, 모멘텀 설정 
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_m, y_m, z_m));
  fParticleGun->GeneratePrimaryVertex(anEvent);  

  delete navigator; 
}
*/