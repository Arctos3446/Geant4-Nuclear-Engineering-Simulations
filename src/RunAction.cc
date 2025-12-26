#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh" 

namespace B1
{

// --------------------------------------------------------------------------
RunAction::RunAction()
{
  // Set up the Analysis Manager (CSV Output).
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");
  analysisManager->SetVerboseLevel(1);
  
  analysisManager->SetFileName("RadShield_Data");

  // ID 0: Energy Spectrum of escaping particles.
  // Range: 0 to 800 keV (perfect for Cs-137 which is 662 keV).
  analysisManager->CreateH1("LeakageSpectrum", "Energy of escaping particles", 100, 0., 800.*keV);

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);
}

// --------------------------------------------------------------------------
void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->OpenFile();

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

// --------------------------------------------------------------------------
void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // 1. Save and Close Data File
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // 2. Merge Accumulables (Combine data from all threads)
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // 3. Compute Dose
  // Energy Deposit (Total and Square for RMS)
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();
  
  // RMS calculation (Standard Deviation)
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  // Get Mass of the Shield (Scoring Volume)
  const auto detConstruction = static_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  
  G4double mass = detConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep / mass;
  G4double rmsDose = rms / mass;

  // 4. Print Executive Report
  if (IsMaster()) {
    G4cout
      << G4endl
      << "################### RADSHIELD SIMULATION REPORT ###################" << G4endl
      << " Simulation Setup:" << G4endl
      << "   - Events Simulated: " << nofEvents << G4endl
      << "   - Shield Mass:      " << G4BestUnit(mass, "Mass") << G4endl
      << G4endl
      << " Dosimetry Results (Shield):" << G4endl
      << "   - Total Absorbed Dose: " << G4BestUnit(dose,"Dose") 
      << " (+/- " << G4BestUnit(rmsDose,"Dose") << ")" << G4endl
      << G4endl
      << " Output:" << G4endl
      << "   - Data saved to: RadShield_Data.csv" << G4endl
      << "###################################################################" << G4endl;
  }
}

// --------------------------------------------------------------------------
void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

}