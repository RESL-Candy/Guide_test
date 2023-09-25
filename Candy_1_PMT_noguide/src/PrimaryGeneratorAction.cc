#include "PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Navigator.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic

  G4String particle_name = "opticalphoton";
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(particle_name);

  fParticleGun->SetParticleDefinition(particleDefinition);
  //fParticleGun->SetParticleEnergy(200.0*GeV); ///////change needed

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  G4LogicalVolume * logicalwater = G4LogicalVolumeStore::GetInstance()->GetVolume("logical_water");

  if (!logicalwater) 
  {
    G4cerr << "Logical volume 'logical_water' not found. Check your geometry setup." << G4endl;
    return;
  }

  G4Navigator* navigator = new G4Navigator();
  navigator->SetWorldVolume(G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume());
  G4ThreeVector randomPosition;
  while (true) 
  {
    randomPosition = G4UniformRand() * (logicalwater->GetSolid()->GetPointOnSurface());

    if (navigator->LocateGlobalPointAndSetup(randomPosition)){
      break;
    }
  } 
  fParticleGun->SetParticlePosition(randomPosition);

  G4double energy = 3.0*eV;
  G4double phi = G4UniformRand() * 2.0 * CLHEP::pi;
  G4double Theta = G4UniformRand() * 2.0 * CLHEP::pi;  // random angle
  
  G4double x_m = G4UniformRand()* std::cos(phi)* std::cos(Theta); // Random x between -1 and 1
  G4double y_m = G4UniformRand()* std::sin(phi)* std::cos(Theta); // Random y between -1 and 1
  G4double z_m = G4UniformRand()* std::sin(Theta); // Random z between -1 and 1


  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_m, y_m, z_m));
  fParticleGun->GeneratePrimaryVertex(anEvent);  

  delete navigator; 
}
