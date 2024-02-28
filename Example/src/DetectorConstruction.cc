//Geant4
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
//User actions
#include "DetectorConstruction.hh"
#include "SphericalParameterisation.hh"



DetectorConstruction::DetectorConstruction() : World_log(0), World_phys(0) {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() 
{
  //Get the material manager
  G4NistManager* nist_manager = G4NistManager::Instance();
  auto water  = nist_manager->FindOrBuildMaterial("G4_WATER");

  //
  //World Volume
  //

  G4Sphere* World = new G4Sphere("World", 0.*CLHEP::cm, 100.*CLHEP::cm, 0.*CLHEP::deg, 360.*CLHEP::deg, 0.*CLHEP::deg, 180.*CLHEP::deg);
  World_log = new G4LogicalVolume(World, water, "World_log", 0, 0, 0);   //G4LogicalVolume* (Solid, Material, Shape)
  
  //Set world to invisible
  G4VisAttributes* w_vis = new G4VisAttributes();
  w_vis->SetColor(1, 0, 0);
  w_vis->SetVisibility(false);
  w_vis->SetForceSolid(false);
  World_log->SetVisAttributes(w_vis);
  
  //Placing World Volume
  World_phys = new G4PVPlacement(0, G4ThreeVector(0,0,0), World_log, "World_phys", 0, false, 0, false);

  //
  //Parameterisation
  //

  //The sphere which will be passed to the parameterisation (the size given here is arbitrary because the parameterisation changes it)
  G4Sphere* ParameterisedSphere = new G4Sphere("ParameterisedSphere", 0.*CLHEP::cm, 5.*CLHEP::cm, 0.*CLHEP::deg, 360.*CLHEP::deg, 0.*CLHEP::deg, 180.*CLHEP::deg);

  //Logical volume for parameterized volume, assign the material here
  G4LogicalVolume* ParameterisedSphereLogical = new G4LogicalVolume(ParameterisedSphere, water, "ParameterisedSphere_log", 0, 0, 0);

  //SphericalParameterisation constructor: start radius, end radius, radial thickness, start phi, end phi, delta phi, start theta, end theta, delta theta
  //Alternatively you can just provide the first three parameters, if you only want to parameterize radius. phi and theta will be set to 0-360 and 0-180 respectively
  SphericalParameterisation* SphericalParam = new SphericalParameterisation(0*cm,5*cm,1*cm,0*degree,180*degree,45*degree,0*degree,180*degree,45*degree);

  //Set the safety parameters of your parameterisation (these are the gaps left between adjacent voxels to prevent overlaps, tweak these if you have issues)
  SphericalParam->SetRadiusSafety(1e-3*nm);
  SphericalParam->SetThetaSafety(0);
  SphericalParam->SetPhiSafety(0);

  //Set parameterisation color to white, and visible in wireframe
  G4VisAttributes* param_vis = new G4VisAttributes();
  w_vis->SetColor(1, 1, 1);
  w_vis->SetVisibility(true);
  w_vis->SetForceSolid(false);
  ParameterisedSphereLogical->SetVisAttributes(param_vis);

  SphericalParam->Place("SphericalParameterisation", ParameterisedSphereLogical, World_log);

  //Call this to print out the information of each voxel
  SphericalParam->PrintComputeDimensions();

  return World_phys;
}
