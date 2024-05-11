#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorManager.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "G4SystemOfUnits.hh"
using CLHEP::keV;

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
    m_energyHistogram = new EnergyHistogram(16384, 0.0, 16.3840);
}


ActionInitialization::~ActionInitialization()
{
    m_energyHistogram->Write("./sim.root");
    delete m_energyHistogram;
}


void ActionInitialization::BuildForMaster() const
{
}


void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorManager());

    auto eventAction = new EventAction(m_energyHistogram);
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction));
}
