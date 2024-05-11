#include "ColdTrap.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

#include "G4Polycone.hh"
#include "G4Tubs.hh"

#include "G4SubtractionSolid.hh"

#include "CLHEP/Units/SystemOfUnits.h"

using CLHEP::cm;
using CLHEP::mm;
using CLHEP::um;
using CLHEP::pi;
using CLHEP::deg;
using CLHEP::perCent;
using CLHEP::cm3;
using CLHEP::g;

/* #include <stdexcept> */
/* using std::runtime_error; */

ColdTrap::ColdTrap() : GeometryObject("ColdTrap")
{
}

ColdTrap::~ColdTrap()
{
}

G4VPhysicalVolume *ColdTrap::Construct()
{
	/// Materials
	//
	G4NistManager* man = G4NistManager::Instance();
	
	G4Material* matCu = man->FindOrBuildMaterial("G4_Cu");
	G4Material* matPol = man->FindOrBuildMaterial("G4_POLYETHYLENE");

	/// Dimensions
	//

	const auto tolerance = 10*um;

	// Cold Trap
	const G4double lengthColdTrap = -380 * mm;
	const G4double rInnerColdTrap = 0.5 * 25 * mm;
	const G4double rOuterColdTrap = 0.5 * 28 * mm;

	// Plastic Support
	const G4double lengthSupport = -19 * mm;
	const G4double rOuterSupport = 0.5 * 35 * mm;
	const G4double rInnerSupport = 0.5 * 28 * mm;

	const G4double phiStart = 0*deg;
	const G4double phiTotal = 360*deg;

	/// Cold Trap
	const  G4int  trapNZPlanes = 2;
	const  G4double trapZPlanes[] =
	{
	        -10 * mm,
		lengthColdTrap
	};

	const G4double trapRInner[] =
	{
		rInnerColdTrap,
		rInnerColdTrap
	};

	const G4double trapROuter[] =
	{
		rOuterColdTrap,
		rOuterColdTrap
	};

	/// Support
	const  G4int  suppNZPlanes = 2;
	const  G4double suppZPlanes[] =
	{
	        -15 * mm,
		lengthSupport
	};

	const G4double suppRInner[] =
	{
		rInnerSupport + tolerance,
		rInnerSupport + tolerance
	};

	const G4double suppROuter[] =
	{
		rOuterSupport,
		rOuterSupport
	};

	/// Volumes
	//

	/// Solids
	G4Polycone* solidTrap = new G4Polycone("Trap",phiStart,phiTotal,trapNZPlanes,
			trapZPlanes,trapRInner,trapROuter);

	G4Polycone* solidSupport = new G4Polycone("Support",phiStart,phiTotal,suppNZPlanes,
			suppZPlanes,suppRInner,suppROuter);

	/// Logical
	G4LogicalVolume* logicTrap = new G4LogicalVolume(solidTrap, matCu,
			solidTrap->GetName());

	G4LogicalVolume* logicSupport = new G4LogicalVolume(solidSupport, matPol,
			solidSupport->GetName());

	/// Visualization attributes
	logicTrap->SetVisAttributes(G4VisAttributes(G4Colour::Brown()));

	logicSupport->SetVisAttributes(G4VisAttributes(G4Colour::White()));

	/// Place as Physical Volumes
	PlaceVolume(logicTrap, GetMotherVolume(),
			-G4ThreeVector(0.,0.,0.));

	PlaceVolume(logicSupport, GetMotherVolume(),
			-G4ThreeVector(0.,0.,0.));

	return nullptr;
}

void ColdTrap::SetNewValue(G4UIcommand* command, G4String value)
{
    GeometryObject::SetNewValue(command, value);
}
