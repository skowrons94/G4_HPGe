#include "TargetChamberC12.hh"

#include <list>

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

#include "G4Polycone.hh"
#include "G4Tubs.hh"

#include "G4SubtractionSolid.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include "G4Colour.hh"

using CLHEP::cm;
using CLHEP::mm;
using CLHEP::um;
using CLHEP::pi;
using CLHEP::deg;
using CLHEP::perCent;
using CLHEP::cm3;
using CLHEP::g;


TargetChamberC12::TargetChamberC12() : GeometryObject("TargetChamberC12")
{
}

TargetChamberC12::~TargetChamberC12()
{
}

G4VPhysicalVolume *TargetChamberC12::Construct()
{
    /// Dimensions
    //

    // CF100 flange upstream

    const auto lengthCF100 = 18 * mm;
    const auto rInnerCF100 = 0.5 * 37  * mm; // Assumed everything is a cylinder with an inner radius of 37 mm
    const auto rOuterCF100 = 0.5 * 114 * mm;

    // CF 40 flange

    const auto lengthCF40 =       13 * mm;
    const auto rInnerCF40 = 0.5 * 37 * mm;
    const auto rOuterCF40 = 0.5 * 70 * mm;

    // Tube

    const auto lengthTube =     234 * mm;
    const auto rInnerTube = 0.5* 37 * mm;
    const auto rOuterTube = 0.5* 40 * mm;

    // Threaded counter flange for target holder installation

    const auto lengthCounterFlange = 12.7 * mm;
    const auto rInnerCounterFlange = 0.5 * 38 * mm;
    const auto rOuterCounterFlange = 0.5 * 50.1 * mm - 1 * um;

    const auto phiStart =   0*deg;
    const auto phiTotal = 360*deg;

    // Chamber itself

    const G4int chamberNZPlanes = 4;
    const G4double chamberZPlanes[] =
    {
        lengthCF100+lengthCF40,
        lengthCF100+lengthCF40+lengthTube,
        lengthCF100+lengthCF40+lengthTube,
        lengthCF100+lengthCF40+lengthTube+lengthCounterFlange
    };

    const G4double chamberRInner[] =
    {
        rInnerTube,
        rInnerTube,
        rInnerCounterFlange,
        rInnerCounterFlange
    };

    const G4double chamberROuter[] =
    {
        rOuterTube,
        rOuterTube,
        rOuterCounterFlange,
        rOuterCounterFlange
    };


    /// Materials
    //

    auto man = G4NistManager::Instance();

    G4Material* matFe = man->FindOrBuildMaterial("G4_Fe");
    G4Material* matCr = man->FindOrBuildMaterial("G4_Cr");
    G4Material* matNi = man->FindOrBuildMaterial("G4_Ni");
    G4Material* matMo = man->FindOrBuildMaterial("G4_Mo");

    G4Material* matSteel = new G4Material("Steel-316", 8.0*g/cm3, 4);
    matSteel->AddMaterial(matFe, 69*perCent);
    matSteel->AddMaterial(matCr, 17*perCent);
    matSteel->AddMaterial(matNi, 12*perCent);
    matSteel->AddMaterial(matMo,  2*perCent);

    auto matAl    = man->FindOrBuildMaterial("G4_Al");

    auto matChamber = matSteel;


    /// Solids
    //

    auto solidCF100
        = new G4Tubs("CF100",
                     rInnerCF100, rOuterCF100,
                     0.5*lengthCF100,
                     phiStart,phiTotal);

    auto solidCF40
        = new G4Tubs("CF40",
                     rInnerCF40, rOuterCF40,
                     0.5*lengthCF40,
                     phiStart,phiTotal);

    G4Polycone* solidTargetChamber = new G4Polycone("TargetChamber",phiStart,phiTotal,
            chamberNZPlanes,chamberZPlanes,chamberRInner,chamberROuter);


    /// Logical volumes
    //

    G4LogicalVolume* logicCF100 = new G4LogicalVolume(solidCF100, matSteel,
            solidCF100->GetName());

    G4LogicalVolume* logicCF40 = new G4LogicalVolume(solidCF40, matSteel,
            solidCF40->GetName());

    G4LogicalVolume* logicTargetChamber = new G4LogicalVolume(solidTargetChamber, matChamber,
            solidTargetChamber->GetName());


    /// Visualization attributes
    //
    logicCF100->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
    logicCF40->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
    logicTargetChamber->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));


    /// Place as Physical Volumes
    //

    PlaceVolume(logicCF100, GetMotherVolume(),
                -G4ThreeVector(0.0, 0.0, 0.5*lengthCF100 + lengthCF40 + lengthTube + lengthCounterFlange));

    PlaceVolume(logicCF40, GetMotherVolume(),
                -G4ThreeVector(0.0, 0.0, 0.5*lengthCF40 + lengthTube + lengthCounterFlange));

    PlaceVolume(logicTargetChamber, GetMotherVolume(), -G4ThreeVector(0.0, 0.0, lengthCF100 + lengthCF40 + lengthTube + lengthCounterFlange));

    return nullptr;
}

void TargetChamberC12::SetNewValue(G4UIcommand* command, G4String value)
{
    GeometryObject::SetNewValue(command, value);
}
