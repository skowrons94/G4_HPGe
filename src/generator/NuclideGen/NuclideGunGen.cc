//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "generator/NuclideGun/NuclideGunGen.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include <memory>
using std::make_shared;

#include <stdexcept>
using std::runtime_error;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuclideGunGen::NuclideGunGen()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   m_position(0, 0, 0)
{

  m_setPositionCmd = make_shared<G4UIcmdWith3VectorAndUnit>("/PrimaryGenerator/NuclideGun/position", this);
  m_setPositionCmd->SetGuidance("Set position of primary vertex.");
  m_setPositionCmd->SetParameterName("x", "y", "z", false);
  m_setPositionCmd->SetUnitCategory("Length");

  m_selectMassCmd = make_shared<G4UIcmdWithADouble>("/PrimaryGenerator/NuclideGun/mass", this);
  m_selectMassCmd->SetGuidance("Select the mass of the nuclide.");
  m_selectChargeCmd = make_shared<G4UIcmdWithADouble>("/PrimaryGenerator/NuclideGun/charge", this);
  m_selectChargeCmd->SetGuidance("Select the charge of the nuclide.");
  
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fParticleGun->SetParticleEnergy(0*eV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));   \

  rand = new TRandom( );       
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuclideGunGen::~NuclideGunGen()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuclideGunGen::GeneratePrimaries(G4Event* anEvent)
{
   // Sampling the beamspot
  theta = rand->Rndm( )*M_PI;
  offset = rand->Rndm( )*beam_sigma - beam_sigma/2;
  offsetX = offset*std::cos( theta ) * mm;
  offsetY = offset*std::sin( theta ) * mm;

  m_position_rand.setX( m_position.getX( ) + offsetX );
  m_position_rand.setY( m_position.getX( ) + offsetY );
  m_position_rand.setZ( m_position.getZ( ) );

  fParticleGun->SetParticlePosition( m_position_rand );

  if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino()) {  
    G4int Z = m_charge, A = m_mass;
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 0.*keV;
    
    G4ParticleDefinition* ion
       = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(ionCharge);
  }    
  //create vertex
  //   
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuclideGunGen::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == m_selectMassCmd.get())
    {
        m_mass = m_selectMassCmd->GetNewDoubleValue(newValue);
    }
    else if(command == m_selectChargeCmd.get())
    {
        m_charge = m_selectChargeCmd->GetNewDoubleValue(newValue);;
    }
    else if (command == m_setPositionCmd.get())
    {
        m_position = m_setPositionCmd->GetNew3VectorValue(newValue);
    }
    else
    {
        throw runtime_error("Unknown command in NuclideGunGen::SetNewValue()");
    }
}
