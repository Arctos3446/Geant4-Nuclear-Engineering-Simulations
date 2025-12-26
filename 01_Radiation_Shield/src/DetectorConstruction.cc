#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

namespace B1
{

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define Materials.

  G4NistManager* nist = G4NistManager::Instance();

  G4Material* mat_air  = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* mat_lead = nist->FindOrBuildMaterial("G4_Pb");

  // Check if materials were correctly found
  if (!mat_air || !mat_lead) {
      G4Exception("DetectorConstruction::Construct()", "Mat01", FatalException, "Material not found in NIST DB");
  }

  // Define World Volume.

  G4double world_size = 1.0 * m;

  auto solidWorld = new G4Box("World", 
                              0.5 * world_size, 
                              0.5 * world_size, 
                              0.5 * world_size);

  auto logicWorld = new G4LogicalVolume(solidWorld, 
                                        mat_air, 
                                        "World");

  auto physWorld = new G4PVPlacement(nullptr,           
                                     G4ThreeVector(),   // at (0,0,0).
                                     logicWorld,        
                                     "World",           
                                     nullptr,           // mother volume.
                                     false,             
                                     0,                 
                                     true);             

  // Define Shielding (Lead Wall).

  G4double shield_thickness = 5.0 * cm; 
  G4double shield_height    = 20.0 * cm;
  G4double shield_width     = 20.0 * cm;

  auto solidShield = new G4Box("Shield", 
                               shield_thickness, 
                               shield_height, 
                               shield_width);

  auto logicShield = new G4LogicalVolume(solidShield, 
                                         mat_lead, 
                                         "LogicShield");

  new G4PVPlacement(nullptr, 
                    G4ThreeVector(0,0,0), 
                    logicShield, 
                    "PhysShield", 
                    logicWorld,       // Mother: World.
                    false, 
                    0, 
                    true);

  // Define Cavity (Hollow inside the shield).

  G4double cavity_thickness = 3.0 * cm; 
  G4double cavity_height    = 18.0 * cm;
  G4double cavity_width     = 18.0 * cm;

  auto solidCavity = new G4Box("Cavity", 
                               cavity_thickness, 
                               cavity_height, 
                               cavity_width);

  auto logicCavity = new G4LogicalVolume(solidCavity, 
                                         mat_air,       // Filled with Air.
                                         "LogicCavity");

  // Inside the Shield (Mother LogicShield).
  new G4PVPlacement(nullptr, 
                    G4ThreeVector(0,0,0), 
                    logicCavity, 
                    "PhysCavity", 
                    logicShield,      // Mother: Shield (Lead).
                    false, 
                    0, 
                    true);


  // Visualization. 
  
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  auto visShield = new G4VisAttributes(G4Color(0.0, 0.0, 1.0, 0.3)); 
  visShield->SetVisibility(true);
  visShield->SetForceSolid(true);
  logicShield->SetVisAttributes(visShield);

  auto visCavity = new G4VisAttributes(G4Color(0.0, 1.0, 1.0)); 
  visCavity->SetVisibility(true);
  visCavity->SetForceWireframe(true);
  logicCavity->SetVisAttributes(visCavity);

  fScoringVolume = logicShield; 

  return physWorld;
}

}