# SphericalParameterisation
A G4VPVParameterisation which slices a sphere in radius, phi, and theta, for use with Geant4 applications. The SphericalParameterisation constructor is as follows: 
```
SphericalParameterisation(const G4double innerRadius, const G4double outerRadius, const G4double radialSegmentThickness, const G4double startPhi = 0*degree, const G4double endPhi = 360*degree, const G4double phiSegmentDegrees = 360*degree, const G4double startTheta = 0*degree, const G4double endTheta = 180*degree, const G4double thetaSegmentDegrees = 180*degree);
```
You only have to set the first three parameters (inner radius, outer radius, and radial segment thickness) if you wish to parameterize the sphere as a function of radius. Otherwise you can provide the additional optional parameters to split the sphere as a function of phi and theta as well.
## Using SphericalParameterisation
An example usage of the SphericalParameterisation is as follows. Start by creating the sphere which will be passed to the parameterisation. The size of the sphere at this point is arbitrary because the parameterisation changes it:
```
G4Sphere* ParameterisedSphere = new G4Sphere("ParameterisedSphere", 0.*CLHEP::cm, 5.*CLHEP::cm, 0.*CLHEP::deg, 360.*CLHEP::deg, 0.*CLHEP::deg, 180.*CLHEP::deg);
```
Then create the logical volume for your parameterized sphere, this is where you set the material:
```
G4LogicalVolume* ParameterisedSphereLogical = new G4LogicalVolume(ParameterisedSphere, water, "ParameterisedSphere_log", 0, 0, 0);
```
As described before, the SphericalParameterisation constructor has 9 input parameters, but you only need to set the first 3 if you just want to create radial slices. In the following example, the sphere goes from 0-5 cm, with 1 cm radial slices, phi and theta go from 0-180 degrees (it's a hemisphere) with 45 degree thick spherical segments.
```
SphericalParameterisation* SphericalParam = new SphericalParameterisation(0*cm,5*cm,1*cm,0*degree,180*degree,45*degree,0*degree,180*degree,45*degree);
```
The Geant4 Navigator can run into issues with parameterisations of curved surfaces that lie next to each other. I have implemented "safety parameters" to leave small gaps between adjacent surfaces to avoid this issue. By default the radial safety parameter is set to a picometer, and the others are set to 0 degrees. The parameters can be set as follows:
```
SphericalParam->SetRadiusSafety(1e-3*nm);
SphericalParam->SetThetaSafety(0);
SphericalParam->SetPhiSafety(0);
```
Finally place your SphericalParameterisation in the world, assuming your mother logical volume is called "World_log":
```
SphericalParam->Place("SphericalParameterisation", ParameterisedSphereLogical, World_log);
```
If you wish to shift the location your SphericalParameterisation is placed, provide a G4ThreeVector when you place the mother logical volume, as the parameterisation should  be placed in the center of whichever mother volume is specified (I have not tested this behavior).
## Example program
For an example of using the SphericalParameterisation, please see the Example folder of the project. To compile and run the project, use the following commands from a Bash terminal:
```
#from inside the Example folder
mkdir build
cd build
cmake ..
make
./SphericalParameterisationExample ../test.mac
```

## Testing SphericalParameterisation
The SphericalParameterisation includes a function *PrintComputeDimensions()* to verify that your spherical volumes have been sliced correctly. This function can be invoked any time after your SphericalParameterisation has been constructed. You can use function in two ways:
```
SphericalParam->PrintComputeDimensions(); //prints out information for every sliced spherical segment you have created
SphericalParam->PrintComputeDimensions(G4int N); //print out only information for the Nth voxel (if there is one)
```
I encourage you to use this function to make sure that the parameterisation has been conducted correctly and that the copy numbers map to the volumes you expect them to.
## Software License
Those who wish to use, modify, or reproduce this software are asked to contact Joseph DeCunha at jdecunha@mdanderson.org. Hearing from you lets me know that I should keep creating useful tools for the community.

Copyright is claimed by Joseph M. DeCunha, 2022. All rights not expressly granted under this license are reserved.

This software is provided by the copyright holder "As is" and any express or implied warranties, including, but not limited to, implied warranties of merchantability, of satisfactory quality, and fitness for a particular purpose or use are disclaimed. The copyright holder makes no representation that the software and modifications thereof, will not infringe any patent, copyright, trade secret or other proprietary right.

The copyright holder shall have no liability for direct, indirect, special, incidental, consequential, exemplary, or punitive damages of any character including, without limitation, procurement of substitute goods or services, loss of use, data or profits, or business interruption, however caused and on any theory of contract, warranty, tort (including negligence), product liability or otherwise, arising in any way out of the use of this software, even if advised of the possibility of such damages.
