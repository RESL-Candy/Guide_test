#include "PhysicsList.hh"


  
MyPhysicsList::MyPhysicsList()
{
  new FTFP_BERT;
  RegisterPhysics (new G4EmStandardPhysics());
  RegisterPhysics (new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{
}