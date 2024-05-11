/// \file IsotropicGunGen.hh
/// \brief Definition of the IsotropicGunGen class

#ifndef PrimaryGunGen_h
#define PrimaryGunGen_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "TRandom.h"
#include "G4ThreeVector.hh"

#include "G4UImessenger.hh"

#include <TCanvas.h>
#include <TH1D.h>

#include <memory>
using std::shared_ptr;

class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gun.
///
/// At the moment it only instantiates the particle gun and uses it to create a
/// single particle per event.
/// This means that the /gun/ commands can be used in the macro to set the
/// energy and position of the gamma.
/// The direction of the particle is sampled randomly for every event, thus
/// setting the direction in the macro using /gun/direction will be disregarded.


class PrimaryGunGen : public G4VUserPrimaryGeneratorAction, public G4UImessenger
{
public:
  PrimaryGunGen();
  virtual ~PrimaryGunGen();

  virtual void GeneratePrimaries(G4Event* event);

  void SetNewValue(G4UIcommand* command, G4String newValue);
  
private:
  G4ParticleGun*  fParticleGun;

  G4ThreeVector m_position;
  G4double m_energy;
  G4double m_number;
  G4double m_angle;

  TRandom*             rand;
  double                tau;
  double                 dE;
  double              angle;
  double               dopp;
  double             energy;
  G4ThreeVector   direction;

  double                  k = 0.989534;
  double                 Z1 = 1;
  double                 Z2 = 6;
  double                 M1 = 1.00727647;
  double                 M2 = 14.003074;
  double                  E = 0.380;
  double             deltaE = 13;

  TCanvas*                c1;
  TH1D*                   h1;

  shared_ptr<G4UIcmdWith3VectorAndUnit> m_setPositionCmd;
  shared_ptr<G4UIcmdWithADoubleAndUnit> m_selectEnergyCmd;
  shared_ptr<G4UIcmdWithADouble> m_selectNParticlesCmd;
  shared_ptr<G4UIcmdWithADouble> m_selectAngleCmd;

};

#endif
