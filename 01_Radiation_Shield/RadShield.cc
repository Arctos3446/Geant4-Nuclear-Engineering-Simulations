#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4SteppingVerbose.hh"
#include "Randomize.hh"
#include "QBBC.hh" 

using namespace B1;

int main(int argc, char** argv)
{
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  runManager->SetUserInitialization(new DetectorConstruction());

  auto physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new ActionInitialization());

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( !ui ) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    UImanager->ApplyCommand("/control/execute vis.mac"); 
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}