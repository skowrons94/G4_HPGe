/// \file IsotropicGunGen.cc
/// \brief Implementation of the PrimaryGunGen class

#include "generator/PrimaryGun/PrimaryGunGen.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <math.h>

using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;

#include <memory>
using std::make_shared;

#include <stdexcept>
using std::runtime_error;

PrimaryGunGen::PrimaryGunGen()
    : G4VUserPrimaryGeneratorAction(),
      G4UImessenger(), 
      fParticleGun(nullptr), 
      m_position(0, 0, 0)
{
    m_setPositionCmd = make_shared<G4UIcmdWith3VectorAndUnit>("/PrimaryGenerator/PrimaryGun/position", this);
    m_setPositionCmd->SetGuidance("Set position of primary vertex.");
    m_setPositionCmd->SetParameterName("x", "y", "z", false);
    m_setPositionCmd->SetUnitCategory("Length");

    m_selectEnergyCmd = make_shared<G4UIcmdWithADoubleAndUnit>("/PrimaryGenerator/PrimaryGun/energy", this);
    m_selectEnergyCmd->SetGuidance("Select the energy of the gamma.");
    m_selectEnergyCmd->SetParameterName("energy", false);
    m_selectEnergyCmd->SetUnitCategory("Energy");

    m_selectNParticlesCmd = make_shared<G4UIcmdWithADouble>("/PrimaryGenerator/PrimaryGun/number", this);
    m_selectNParticlesCmd->SetGuidance("Select the number of the gammas.");
    m_selectNParticlesCmd->SetParameterName("N", false);

    m_selectAngleCmd = make_shared<G4UIcmdWithADouble>("/PrimaryGenerator/PrimaryGun/angle", this);
    m_selectAngleCmd->SetGuidance("Select the angle of the gamma.");
    m_selectAngleCmd->SetParameterName("Theta", false);

    rand = new TRandom( );

    tau  = k*Z1*Z2*sqrt( (1/E)*(M1*M2/(M1+M2)) )/(2*E);
    
}

PrimaryGunGen::~PrimaryGunGen()
{
  
    delete fParticleGun;
}

void PrimaryGunGen::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the begining of event

    fParticleGun->SetParticleDefinition(G4Gamma::Gamma());

    direction = G4RandomDirection();
    direction.setTheta( m_angle * deg );
    angle     = direction.angle( G4ThreeVector( 0, 0, 1 ) );
    
    /* if( rand->Rndm( ) > 0.1 ){ 
   
      do{
	dE = rand->Exp( 1/tau )*1e3;
      } while( dE > deltaE );

    }
    else{

      dE = rand->Exp( 1/tau )*1e3;
      dE += deltaE;
      
      }*/
    
    energy = m_energy;// - dE*1e-3;

    dopp      =  4.63367*pow( 10, -2 )*cos( angle )*energy*sqrt(M1*E)/M2;

    energy += dopp;

    //    std::cout << "Angle: " << angle << "   Doppler: " << dopp << std::endl;

    fParticleGun->SetParticleEnergy(energy*CLHEP::MeV);
    fParticleGun->SetParticlePosition(m_position);

    // Set gun direction randomly
    fParticleGun->SetParticleMomentumDirection(direction);

    // Generate the vertex (position + particle) for the event
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGunGen::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == m_setPositionCmd.get())
    {
        m_position = m_setPositionCmd->GetNew3VectorValue(newValue);
    }
    else if(command == m_selectEnergyCmd.get())
    {
        m_energy = m_selectEnergyCmd->GetNewDoubleValue(newValue);
    }
    else if(command == m_selectNParticlesCmd.get())
    {
        m_number = m_selectNParticlesCmd->GetNewDoubleValue(newValue);
	fParticleGun = new G4ParticleGun(m_number);
    }
    else if(command == m_selectAngleCmd.get())
    {
        m_angle = m_selectAngleCmd->GetNewDoubleValue(newValue);
	fParticleGun = new G4ParticleGun(m_number);
    }
    else
    {
        throw runtime_error("Unknown command in GammaDecaySchemeGen::SetNewValue()");
    }
}
