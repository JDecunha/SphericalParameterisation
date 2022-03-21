//Geant4
#include "G4Sphere.hh"
#include "G4ThreeVector.hh"
#include "G4PVParameterised.hh"
#include "G4VPhysicalVolume.hh"
//std library
#include <cmath>
//User actions
#include "SphericalParameterisation.hh"

//
//Defining static objects
//

//This is a global object, which is a member of SphericalParameterisation
//We define it as static so that if you're testing a parameterisation then a ton of G4Sphere test objects don't have to be created
G4Sphere SphericalParameterisation::testSphere = G4Sphere("TestSphere", 0.*CLHEP::cm, 5.*CLHEP::cm, 0.*CLHEP::deg, 360.*CLHEP::deg, 0.*CLHEP::deg, 180.*CLHEP::deg);

//
//Class function definitions
//

SphericalParameterisation::SphericalParameterisation(const G4double innerRadius, const G4double outerRadius, const G4double radialSegmentThickness, const G4double startPhi, const G4double endPhi, const G4double phiSegmentDegrees, const G4double startTheta, const G4double endTheta, const G4double thetaSegmentDegrees)
{
    _innerRadius = innerRadius;
    _outerRadius = outerRadius;
    _radialSegmentThickness = radialSegmentThickness;
    
    _startPhi = startPhi;
    _endPhi = endPhi;
    _phiSegmentDegrees = phiSegmentDegrees;

    _startTheta = startTheta;
    _endTheta = endTheta;
    _thetaSegmentDegrees = thetaSegmentDegrees;

    //Check the requested radius, phi, and theta tile the sphere without cut off voxels
    VerifyUserDimensions(); 

    //Pre-calculate the number of increments
    _radialIncrements = (_outerRadius-_innerRadius)/_radialSegmentThickness;
    _phiIncrements = (_endPhi-_startPhi)/_phiSegmentDegrees;
    _thetaIncrements = (_endTheta-_startTheta)/_thetaSegmentDegrees;
    _nCopies = _thetaIncrements*_phiIncrements*_radialIncrements;

    //Set default values for safety parameters
    _radiusSafety = 1e-3*nm;
    _phiSafety = 0;
    _thetaSafety = 0;
}

void SphericalParameterisation::VerifyUserDimensions()
{
    G4double radiusRange = _outerRadius - _innerRadius;
    G4double phiRange = _endPhi - _startPhi;
    G4double thetaRange = _endTheta - _startTheta;

    //fmod has some error, so even perfectly divisible numbers don't return 0, that's why we have the greater than 0.01
    if (std::fmod(radiusRange,_radialSegmentThickness) > 0.2)
    {
        G4cout << "SphericalParameterisation: radialRange is not divisible by requested radialSegmentThickness." << G4endl; abort();
    }
    if (std::fmod(phiRange,_phiSegmentDegrees) > 0.2)
    {
        G4cout << "SphericalParameterisation: phiRange is not divisible by requested phi SegmentDegrees." << G4endl; abort();
    }
    if (std::fmod(thetaRange,_thetaSegmentDegrees) > 0.2)
    {
        G4cout << "SphericalParameterisation: thetaRange is not divisible by requested thetaSegmentDegrees." << G4endl; abort();
    }
}

void SphericalParameterisation::ComputeDimensions(G4Sphere& sphere, const G4int copyNo, const G4VPhysicalVolume*) const
{
    //Explanation of the logic below.
    //So we're going to iterate over theta, then phi, then radius
    //
    //How often do we increase thetanum? every copy no
    //When do we reset thetanum back to zero? every thetaIncrements
    //
    //How often do we increase phinum? every thetaIncrements
    //When do we reset phinum back to zero? every thetaIncrements*phiIncrements
    //
    //How often do we increase radiusnum? every thetaIncrements*phiIncrements
    //When do we reset radiusnum? never, because we are done iterating

    //Reset thetaNum to zero every _thetaIncrements
    G4int thetaNum = copyNo - (copyNo/_thetaIncrements)*_thetaIncrements;

    //Reset phiNum to zero every _thetaIncrements*_phiIncrements
    G4int phiNum = copyNo - (copyNo/(_thetaIncrements*_phiIncrements))*_thetaIncrements*_phiIncrements;

    //Increments radius num every time phi and theta iterated over
    G4int radiusNum = copyNo/(_phiIncrements*_thetaIncrements);

    //Set the sphere values
    sphere.SetInnerRadius((radiusNum*_radialSegmentThickness+_innerRadius)+_radiusSafety);
    sphere.SetOuterRadius(((radiusNum+1)*_radialSegmentThickness+_innerRadius));

    sphere.SetStartPhiAngle(phiNum*_phiSegmentDegrees+_startPhi);
    sphere.SetDeltaPhiAngle(_phiSegmentDegrees-_phiSafety);

    sphere.SetStartThetaAngle(thetaNum*_phiSegmentDegrees+_startPhi);
    sphere.SetDeltaThetaAngle(_thetaSegmentDegrees-_thetaSafety);

    //Break if too many copies
    if(copyNo >= (_phiIncrements*_thetaIncrements*_radialIncrements)) 
    { 
        G4cout << "Too many copies entered into SphericalParameterisation" << G4endl; 
        abort();
    }
}

void SphericalParameterisation::ComputeTransformation(const G4int, G4VPhysicalVolume* physVol) const
{
    //No rotation or translation relative to Mother Volume
    physVol->SetTranslation(G4ThreeVector(0,0,0));
    physVol->SetRotation(0);
}

void SphericalParameterisation::Place(G4String name, G4LogicalVolume* parameterisation_logical, G4LogicalVolume* mother_logical)
{
    //Place the SphericalParameterisation in the mother_logical, using G4PVParameterised
    new G4PVParameterised(name, parameterisation_logical, mother_logical, kUndefined, _nCopies, this, false);
}

void SphericalParameterisation::PrintComputeDimensions(const G4int copyNo)
{
    //Function for verifying the Compute Dimensions algorithm
    //
    //This function has two different behaviors: if copyNo is set explicitly it returns the dimensions for that copyNo
    //Otherwise if called without a parameter it loops through all of the parameterisations and prints their values.

    //Create an empty pointer to satisfy the G4VPhysicalVolume argument
    G4VPhysicalVolume* dummy = nullptr;

    if (copyNo != -1) //copyNo is specified by user
    {
        ComputeDimensions(SphericalParameterisation::testSphere, copyNo, dummy);

        G4cout << "Dimensions for sphere copy number: " << copyNo << G4endl;

        //Get and print the sphere values
        G4cout << "Inner radius: " << testSphere.GetRmin() << " mm" << G4endl;
        G4cout << "Outer radius: " << testSphere.GetRmax() << " mm" << G4endl;
        G4cout << "Start phi angle: " << testSphere.GetSPhi()/deg << " degrees" << G4endl;
        G4cout << "Delta phi angle: " << testSphere.GetDPhi()/deg << " degrees" << G4endl;
        G4cout << "Start theta angle:  " << testSphere.GetSTheta()/deg << " degrees" << G4endl;
        G4cout << "Delta theta angle: " << testSphere.GetDTheta()/deg << " degrees" << G4endl;
    }
    else //copyNo not explicitly given, loop through all  
    {
        for (int i = 0; i < _nCopies; i++)
        {
            ComputeDimensions(SphericalParameterisation::testSphere, i, dummy);

            G4cout << "Dimensions for sphere copy number: " << i << G4endl;

            //Get and print the sphere values
            G4cout << "Inner radius: " << testSphere.GetRmin() << " mm" << G4endl;
            G4cout << "Outer radius: " << testSphere.GetRmax() << " mm" << G4endl;
            G4cout << "Start phi angle: " << testSphere.GetSPhi()/deg << " degrees" << G4endl;
            G4cout << "Delta phi angle: " << testSphere.GetDPhi()/deg << " degrees" << G4endl;
            G4cout << "Start theta angle:  " << testSphere.GetSTheta()/deg << " degrees" << G4endl;
            G4cout << "Delta theta angle: " << testSphere.GetDTheta()/deg << " degrees" << G4endl;
            G4cout << G4endl;
        }
    }
}


