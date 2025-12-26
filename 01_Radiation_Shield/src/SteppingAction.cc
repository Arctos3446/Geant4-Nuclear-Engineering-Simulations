#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh" 

namespace B1
{

// --------------------------------------------------------------------------
SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

// --------------------------------------------------------------------------
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  //  The Scoring Volume (The Shield).
  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // Get the volume where the particle is CURRENTLY located.
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()
                                           ->GetVolume()->GetLogicalVolume();
 
  // If a particle JUST entered the "World" volume, it means it escaped the Shield.
  // We check for "fGeomBoundary" to ensure we only count it once (the moment it crosses).
  
  if (volume->GetName() == "World" && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
      
      // Get Kinetic Energy of the escaping particle.
      G4double energy = step->GetPreStepPoint()->GetKineticEnergy();

      auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->FillH1(0, energy); 
  }

  // Logic: We only care about energy deposited INSIDE the Shield.
  
  if (volume != fScoringVolume) return;

  // Collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  
  // Sum it up in EventAction.
  fEventAction->AddEdep(edepStep);
}

}