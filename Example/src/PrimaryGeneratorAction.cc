#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() 
{
  particleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() 
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) 
{
  particleGun->GeneratePrimaryVertex(anEvent);
}



