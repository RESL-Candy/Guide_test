#ifndef PHYSICS_h
#define PHYSICS_h

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
class MyPhysicsList : public G4VModularPhysicsList
{
public:
  MyPhysicsList();
  ~MyPhysicsList() override;

};

#endif