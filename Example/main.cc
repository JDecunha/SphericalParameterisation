//Geant4
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

//Standard library
#include <iostream>

//User actions
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv) 
{
    //Fetch the random seed from the terminal arguments
    long int randomSeed = 1;
    if (argc > 2) 
    {
        std::string timestring = argv[2];
        char *pEnd;
        randomSeed = strtol(argv[2], &pEnd, 10);
    }
    else { std::cout << "No random seed given on command line, using seed: " << randomSeed << std::endl; }
    G4Random::setTheSeed(randomSeed); 

    //Create the user actions
    G4RunManager* runManager = new G4RunManager;
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserAction(new PrimaryGeneratorAction());

    //If compiled with visualization abilities
    #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    #endif

    G4UImanager* UI = G4UImanager::GetUIpointer(); //Get the user interface manager 
    if (argc != 1) //Grab and run the macro file if there is one
    {
        UI->ApplyCommand("/control/execute "+(G4String)argv[1]);
    } 
    else { std::cout << "No macro given on command line. " << std::flush; abort(); }

    delete runManager;
    return 0;
}
