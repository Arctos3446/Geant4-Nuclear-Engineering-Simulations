#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh" // Necessary for isotropic emission

namespace B1
{

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  
  // Gamma radiation.
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);

  // Energy 662 keV (Cs-137 peak).
  fParticleGun->SetParticleEnergy(662. * keV);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // We place the source at the center of the cavity (0,0,0).
  // This simulates a radioactive source inside the shielding box.
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));

  // Isotropic emission.
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

}