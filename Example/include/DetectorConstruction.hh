#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

class DetectorMessenger;
class G4LogicalVolume;
class G4Material;
class G4Box;
class G4Orb;
class G4Sphere;
class G4Colour;
class G4VPhysicalVolume;
class G4VPhysicalVolume;
class SourceMaterial;
class SourceModel;

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
  	DetectorConstruction();
  	~DetectorConstruction();
  	G4VPhysicalVolume* Construct();

    private:
    // Logical volumes
    G4LogicalVolume* World_log;
    // Physical volumes
    G4VPhysicalVolume* World_phys;
};

#endif
