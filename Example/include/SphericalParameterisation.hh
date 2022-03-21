#pragma once

//Forward declarations
class G4Sphere;
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Ellipsoid;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;
//Geant4
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPVParameterisation.hh"

class SphericalParameterisation : public G4VPVParameterisation 
{

  public:

    //Constructor
    SphericalParameterisation(const G4double innerRadius, const G4double outerRadius, const G4double radialSegmentThickness, const G4double startPhi = 0*degree, const G4double endPhi = 360*degree, const G4double phiSegmentDegrees = 360*degree, const G4double startTheta = 0*degree, const G4double endTheta = 180*degree, const G4double thetaSegmentDegrees = 180*degree);
    
    //Destructor
    ~SphericalParameterisation() override {}

    //Call this to print the dimensions of the sphere with the given copyNo
    void PrintComputeDimensions(const G4int copyNo = -1); 

    //Call this to place the parameterisation, automatically determining the # of voxels
    void Place(G4String name, G4LogicalVolume* parameterisation_logical, G4LogicalVolume* mother_logical);
    
    //Overidden methods
    void ComputeDimensions(G4Sphere& sphere, const G4int copyNo, const G4VPhysicalVolume*) const override;
    void ComputeTransformation(const G4int, G4VPhysicalVolume* physVol) const override;

    //Getter functions
    G4double GetRadiusSafety() const { return _radiusSafety; }
    G4double GetPhiSafety() const { return _phiSafety; }
    G4double GetThetaSafety() const { return _thetaSafety; }

    //Setter functions
    void SetRadiusSafety(G4double newVal) { _radiusSafety = newVal; }
    void SetPhiSafety(G4double newVal) { _phiSafety = newVal; }
    void SetThetaSafety(G4double newVal) { _thetaSafety = newVal; }
      
  private:

    //Checks the requested radius, phi, and theta tile the sphere without cut off voxels
    void VerifyUserDimensions(); 

    //Values passed in by user (some of these are optional)
    G4double _innerRadius, _outerRadius, _radialSegmentThickness;
    G4double _startPhi, _endPhi, _phiSegmentDegrees;
    G4double _startTheta, _endTheta, _thetaSegmentDegrees;

    //Properties calculated in class
    G4int _radialIncrements, _phiIncrements, _thetaIncrements, _nCopies;

    //Safety parameters, give the margin between two adjacent surfaces:
    G4double _radiusSafety, _phiSafety, _thetaSafety;

    //So that the test function doesn't have to construct a bunch of test spheres
    static G4Sphere testSphere;

    //To hide compiler warning: override all the other inherited virtual declarations of ComputeDimensions
    void ComputeDimensions (G4Box&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Trd&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Tubs&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Trap&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Cons&, const G4int,const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Orb&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Ellipsoid&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Torus&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Para&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Hype&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Polycone&, const G4int, const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Polyhedra&, const G4int, const G4VPhysicalVolume*) const override {}

};
