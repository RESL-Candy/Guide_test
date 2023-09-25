#ifndef PRIMARYGENERATORACTION_h
#define PRIMARYGENERATORACTION_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Alpha.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4RandomTools.hh"

class G4ParticleGun;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  virtual void GeneratePrimaries(G4Event *) override;

  // method to access particle gun
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
    G4ParticleGun *fParticleGun;
};

#endif