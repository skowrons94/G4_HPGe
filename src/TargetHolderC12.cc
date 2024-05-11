#include "TargetHolderC12.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

#include "G4VSolid.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"

#include "G4PVReplica.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4SubtractionSolid.hh"

#include "G4PVPlacement.hh"

#include "G4UIcmdWithAString.hh"

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

#include <stdexcept>
using std::runtime_error;

#include <memory>
using std::make_shared;

TargetHolderC12::TargetHolderC12() : GeometryObject("TargetHolderC12")
{
    fTargetCmd = make_shared<G4UIcmdWithAString>("/Geometry/TargetHolderC12/target", this);
    fTargetCmd->SetGuidance("Set target of the target holder.");
    fTargetCmd->SetParameterName("mode", false);
    fTargetCmd->SetCandidates("graphite source evaporated");
    fTargetCmd->SetToBeBroadcasted(false);
}

TargetHolderC12::~TargetHolderC12()
{
}

void TargetHolderC12::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == fTargetCmd.get())
    {
        fTargetChosen = true;

        if (newValue == "graphite")
        {
            fTarget = targetGraphite;
        }
        else if (newValue == "source")
        {
            fTarget = targetSource;
        }
        else if (newValue == "evaporated")
        {
            fTarget = targetEvaporated;
        }
        else
        {
            G4cout << "Unknown TargetHolderC12 target '" << newValue << "'." << G4endl;
            fTargetChosen = false;
        }
    }
    else
    {
        GeometryObject::SetNewValue(command, newValue);
    }
}

G4VPhysicalVolume *TargetHolderC12::Construct()
{
	/// Materials
	//
	G4NistManager* man = G4NistManager::Instance();

	G4Material* matBrass = man->FindOrBuildMaterial("G4_BRASS");
	G4Material* matCs = man->FindOrBuildMaterial("G4_POLYETHYLENE");
	G4Material* matWater = man->FindOrBuildMaterial("G4_WATER");
	G4Material* matTa = man->FindOrBuildMaterial("G4_Ta");
	G4Material* matAl = man->FindOrBuildMaterial("G4_Al");
	G4Material* matC = man->FindOrBuildMaterial("G4_GRAPHITE");

	G4Material* matFe = man->FindOrBuildMaterial("G4_Fe");
	G4Material* matCr = man->FindOrBuildMaterial("G4_Cr");
	G4Material* matNi = man->FindOrBuildMaterial("G4_Ni");
	G4Material* matMo = man->FindOrBuildMaterial("G4_Mo");

	G4Material* matSteel = new G4Material("Steel-316", 8.0*g/cm3, 4);
	matSteel->AddMaterial(matFe, 69*perCent);
	matSteel->AddMaterial(matCr, 17*perCent);
	matSteel->AddMaterial(matNi, 12*perCent);
	matSteel->AddMaterial(matMo,  2*perCent);

	/// Dimensions
	//
	
	G4RotationMatrix noRotation;
	const auto tolerance = -10*um;

	G4RotationMatrix xRot;
	xRot.rotateX(90.*deg);
	xRot.rotateZ(0.5*45.*deg);

	// Target Holder Scheme Dimensions
	const G4double lengthHolderTop = -26.2 * mm;
	const G4double rInnerHolderTop = 0.5 * 50.1 * mm;
	const G4double rOuterHolderTop = 0.5 * 69.0 * mm;

	const G4double lengthHolderInside = -16.2 * mm;
	const G4double rInnerHolderInside = 0.5 * 43.5 * mm;
	const G4double rOuterHolderInside = 0.5 * 50.3 * mm;

	const G4double lengthHolderHole = -12.5 * mm;
	const G4double rInnerHolderHole = 0.5 * 23.3 * mm;
	const G4double rOuterHolderHole = 0.5 * 43.5 * mm;

	const G4double lengthHolderBack = -1 * mm;
	const G4double rInnerHolderBack = 0.5 * 0 * mm;
	const G4double rOuterHolderBack = 0.5 * 69 * mm;

	// Ta Backing Dimensions
	const G4double lengthBacking = -0.4 * mm;
	const G4double rInnerBacking = 0.5 * 0.0 * mm;
	const G4double rOuterBacking = 0.5 * 43.5 * mm;

	// Cesium Source
	const G4double lengthTargetCs = -0.5 * mm;
	const G4double rInnerTargetCs = 0.5 * 0 * mm;
	const G4double rOuterTargetCs = 0.5 * 23.5 * mm;

	// Graphite Target
	const G4double lengthTargetGraph = -6.0 * mm;
	const G4double rInnerTargetGraph = 0.5 * 0 * mm;
	const G4double rOuterTargetGraph = 0.5 * 25 * mm;

	// Graphite Target Support
	const G4double lenghtGraphiteSupportHZ = 0.2 * mm;
	const G4double lenghtGraphiteSupportHX = 1.0 * mm;
	const G4double lenghtGraphiteSupportHY = 1.3 * mm;

	const G4double lenghtGraphiteSupportVZ = 3 * mm;
	const G4double lenghtGraphiteSupportVX = 0.2 * mm;
	const G4double lenghtGraphiteSupportVY = 1.3 * mm;

	// Water Cooling Dimensions
	const G4double lengthWater = -10.0 * mm;
	const G4double rInnerWater = 0.5 * 0 * mm;
	const G4double rOuterWater = 0.5 * 23.2 * mm;

	// M2.5 Backing Dimension
	const G4double lenghtM2Backing = - 1 * mm;
	const G4double rInnerScrewBacking= 0.5 * 27.3 * mm;

	// Holes for M2.5 Screws Dimensions
	const G4double lengthM2Holes = 4.0 * mm;
	const G4double rM2Holes = 0.5 * 2.5 * mm;
	const G4double centerM2Holes = ( rOuterHolderHole + rInnerScrewBacking )/2;
	const G4double phiM2Holes = 45*deg;

	// M2.5 Screws Dimensions
	const G4double lengthM2Screw = 1.6 * mm;
	const G4double rM2Screw = 0.5 * 4.5 * mm;

	// Water Cooling Holes Dimensions
	const G4double rWaterHoles = 0.5 * 7.1 * mm;
	const G4double rWaterHolesInside = 0.5 * 3.5 * mm;
	const G4double centerWaterHoles = -8.1 * mm;

	// Water Tubes Dimensions
	const G4double lengthWaterTubes = 25.0 * cm;
	const G4double rOuterWaterTubes = rWaterHoles + tolerance;
	const G4double rInnerWaterTubes = rOuterWaterTubes - 0.5 * mm;

	// Water Tubes Screws
	const G4double lengthWaterScrews = 4.0 * mm;
	const G4double rOuterWaterScrews = rOuterWaterTubes + 2 * mm;
	const G4double rInnerWaterScrews = rOuterWaterTubes - tolerance;

	const G4double phiStart = 0*deg;
	const G4double phiTotal = 360*deg;

	/// Target Holder
	const  G4int  holderNZPlanes = 9;
	const  G4double holderZPlanes[] =
	{
		0,
		lengthHolderBack,
		lengthHolderBack,
		lengthHolderHole,
		lengthHolderHole,
		lengthHolderInside,
		lengthHolderInside,
		lengthHolderTop,
		lengthHolderTop
	};

	const G4double holderRInner[] =
	{
		rInnerHolderBack,
		rInnerHolderBack,
		rInnerHolderHole,
		rInnerHolderHole,
		rInnerHolderInside,
		rInnerHolderInside,
		rInnerHolderTop,
		rInnerHolderTop,
		rInnerHolderTop
	};

	const G4double holderROuter[] =
	{
		rOuterHolderBack,
		rOuterHolderBack,
		rOuterHolderBack,
		rOuterHolderTop,
		rOuterHolderTop,
		rOuterHolderTop,
		rOuterHolderTop,
		rOuterHolderTop,
		rOuterHolderTop	
	};

	/// M2.5 Screw
	const  G4int  screwNZPlanes = 5;
	const  G4double screwZPlanes[] =
	{
		lengthHolderHole + lengthM2Holes,
		lengthHolderHole + tolerance,
		lengthHolderHole + tolerance,
		lengthHolderHole - lengthM2Screw,
		lengthHolderHole - lengthM2Screw

	};

	const G4double screwRInner[] =
	{
		0,
		0,
		0,
		0,	
		0
	};

	const G4double screwROuter[] =
	{
		rM2Holes + tolerance,
		rM2Holes + tolerance,
		rM2Screw,
		rM2Screw,
		rM2Screw
	};

	/// Target Holder M2.5 Holes
	G4Tubs* screwHoles = new G4Tubs("M2.5_Holes", 0.*mm, rM2Holes, lengthM2Holes, 0., 2.*pi);

	/// Ta Backing
	const G4int backingNZPlanes = 2;
	const G4double backingZPlanes[] =
	{
		lengthHolderHole + tolerance,
		lengthHolderHole + lengthBacking + tolerance
	};

	const G4double backingRInner[] = {rInnerBacking,rInnerBacking};
	const G4double backingROuter[] = {rOuterBacking + tolerance,rOuterBacking + tolerance};

	/// M2.5 Screw Backing
	const G4int screwBackingNZPlanes = 2;
	const G4double screwBackingZPlanes[] =
	{
		lengthHolderHole + lengthBacking + 2 * tolerance,
		lengthHolderHole + lenghtM2Backing + lengthBacking + 2 * tolerance
	};

	const G4double screwBackingRInner[] = {rInnerScrewBacking,rInnerScrewBacking};
	const G4double screwBackingROuter[] = {rOuterBacking + tolerance,rOuterBacking + tolerance};

	/// Graphite Target
	const G4int targetGraphNZPlanes = 2;
	const G4double targetGraphZPlanes[] =
	{
		lengthHolderHole + lengthBacking + 2 * tolerance,
		lengthHolderHole + lengthBacking + 2 * tolerance + lengthTargetGraph
	};

	const G4double targetGraphRInner[] = {rInnerTargetGraph,rInnerTargetGraph};
	const G4double targetGraphROuter[] = {rOuterTargetGraph,rOuterTargetGraph};

	/// Cesium Source
	const G4int targetCsNZPlanes = 2;
	const G4double targetCsZPlanes[] =
	{
		lengthHolderHole + lengthBacking + 2 * tolerance,
		lengthHolderHole + lengthBacking + 2 * tolerance + lengthTargetCs
	};

	const G4double targetCsRInner[] = {rInnerTargetCs,rInnerTargetCs};
	const G4double targetCsROuter[] = {rInnerHolderHole + tolerance,rInnerHolderHole + tolerance};

	/// Water
	const G4int waterNZPlanes = 2;
	const G4double waterZPlanes[] =
	{
		lengthHolderBack + tolerance,
		lengthWater + lengthHolderBack
	};

	const G4double waterRInner[] = {rInnerWater,rInnerWater};
	const G4double waterROuter[] = {rOuterWater + tolerance,rOuterWater + tolerance};

	/// Water Screws
	const G4int waterScrewsNZPlanes = 2;
	const G4double waterScrewsZPlanes[] =
	{
		tolerance,
		lengthWaterScrews
	};

	const G4double waterScrewsRInner[] = {rInnerWaterScrews,rInnerWaterScrews};
	const G4double waterScrewsROuter[] = {rOuterWaterScrews,rOuterWaterScrews};

	/// Target Holder Water Holes
	G4Tubs* waterHoles = new G4Tubs( "Water_Holes", 0.*mm, rWaterHoles, 12.0 * mm , 0., 2.*pi);
	G4Tubs* waterHolesInside = new G4Tubs( "Water_Holes2", 0.*mm, rWaterHolesInside, 21 * mm , 0., 2.*pi);

	/// Target Holder Water Tubes
	G4Tubs* waterTubes = new G4Tubs(GetName() + "Water_Tubes", rInnerWaterTubes, rOuterWaterTubes, lengthWaterTubes, 0., 2.*pi);

	/// Target Holder Water Tubes Water
	G4Tubs* waterTubesWater = new G4Tubs(GetName() + "Water_Tubes", 0.*mm, rWaterHoles + tolerance, 0.5 * 12.0 * mm, 0., 2.*pi);
	G4Tubs* waterTubesWaterInner = new G4Tubs(GetName() + "Water_Tubes", 0.*mm, rWaterHolesInside + tolerance, 0.5 * 10.8 * mm, 0., 2.*pi);

	/// Volumes
	//

	/// Target Holder
	G4Polycone* solidHolderPl = new G4Polycone("Holder",phiStart,phiTotal,holderNZPlanes,
			holderZPlanes,holderRInner,holderROuter);

	// Now the M2.5 holes are cut in the Target Holder
	G4VSolid* solidHolder = solidHolderPl;
	for( int i = 0; i < 8; ++i ){  
		solidHolder = new G4SubtractionSolid("Target_Holder_cut", solidHolder, screwHoles,
            								  G4Transform3D(noRotation,
                          						                G4ThreeVector(centerM2Holes*std::sin(i*phiM2Holes), 
										  		      centerM2Holes*std::cos(i*phiM2Holes), 
												      lengthHolderHole + lengthM2Holes + tolerance)));
	};

	// Now the Water Holes are cut
	solidHolder = new G4SubtractionSolid("Target_Holder_body", solidHolder, waterHoles,
            							G4Transform3D(xRot,
                          				G4ThreeVector(-rOuterHolderTop*std::sin(0.5*45.*deg), rOuterHolderTop*std::cos(0.5*45.*deg), centerWaterHoles)));

	solidHolder = new G4SubtractionSolid("Target_Holder_body", solidHolder, waterHoles,
            							G4Transform3D(xRot,
                          				G4ThreeVector(rOuterHolderTop*std::sin(0.5*45.*deg), -rOuterHolderTop*std::cos(0.5*45.*deg), centerWaterHoles)));

	solidHolder = new G4SubtractionSolid("Target_Holder_body", solidHolder, waterHolesInside,
            							G4Transform3D(xRot,
                          				G4ThreeVector(-( rOuterHolderTop - 12 )*std::sin(0.5*45.*deg), ( rOuterHolderTop - 12 )*std::cos(0.5*45.*deg), centerWaterHoles)));

	solidHolder = new G4SubtractionSolid("Target_Holder_body", solidHolder, waterHolesInside,
            							G4Transform3D(xRot,
                          				G4ThreeVector(( rOuterHolderTop - 12 )*std::sin(0.5*45.*deg), -(rOuterHolderTop - 12 )*std::cos(0.5*45.*deg), centerWaterHoles)));

	// Graphite Target
	G4Polycone* solidTargetC = new G4Polycone("Target",phiStart,phiTotal,targetGraphNZPlanes,
			targetGraphZPlanes,targetGraphRInner,targetGraphROuter);

	G4Box* solidGraphiteSupportH = new G4Box("GraphiteSupportH", lenghtGraphiteSupportHX, lenghtGraphiteSupportHY, lenghtGraphiteSupportHZ );
	G4Box* solidGraphiteSupportV = new G4Box("GraphiteSupportV", lenghtGraphiteSupportVX, lenghtGraphiteSupportVY, lenghtGraphiteSupportVZ );

	// Cesium Source
	G4Polycone* solidTargetCs = new G4Polycone("Target",phiStart,phiTotal,targetCsNZPlanes,
			targetCsZPlanes,targetCsRInner,targetCsROuter);

	// Ta Backing
	G4Polycone* solidBackingPl = new G4Polycone("Backing",phiStart,phiTotal,backingNZPlanes,
			backingZPlanes,backingRInner,backingROuter);

	//Now the M2.5 Holes are cut in the Ta Backing
	G4VSolid* solidBacking = solidBackingPl;
	for( int i = 0; i < 8; ++i ){
		solidBacking = new G4SubtractionSolid("Backing_cut", solidBacking, screwHoles,
            								   G4Transform3D(noRotation,
                          						                 G4ThreeVector(centerM2Holes*std::sin(i*phiM2Holes), 
										  		       centerM2Holes*std::cos(i*phiM2Holes), 
												       lengthHolderHole + lengthBacking + 10*tolerance + lengthM2Holes)));
	};

	// M2.5 Screw Backing
	G4Polycone* solidScrewBackingPl = new G4Polycone("ScrewBacking",phiStart,phiTotal,screwBackingNZPlanes,
			screwBackingZPlanes,screwBackingRInner,screwBackingROuter);

	// Now the M2.5 Holes are cut in the Screw Backing
	G4VSolid* solidScrewBacking = solidScrewBackingPl;
	for( int i = 0; i < 8; ++i ){
		solidScrewBacking = new G4SubtractionSolid( "Screw_Backing_body", solidScrewBacking, screwHoles,
            									G4Transform3D(noRotation,
                          						                      G4ThreeVector(centerM2Holes*std::sin(i*phiM2Holes), 
										  			    centerM2Holes*std::cos(i*phiM2Holes), 
													    lengthHolderHole + lenghtM2Backing + lengthBacking + 10 * tolerance + lengthM2Holes)));
	};

	// Water Cooling
	G4Polycone* solidWater = new G4Polycone("Water",phiStart,phiTotal,waterNZPlanes,
			waterZPlanes,waterRInner,waterROuter);

	G4Polycone* solidWaterScrews = new G4Polycone("WaterScrews",phiStart,phiTotal,waterScrewsNZPlanes,
			waterScrewsZPlanes,waterScrewsRInner,waterScrewsROuter);

	// M2.5 Screw
	G4Polycone* solidScrew = new G4Polycone("Screw",phiStart,phiTotal,screwNZPlanes,
			screwZPlanes,screwRInner,screwROuter);

	/// Logical
	G4LogicalVolume* logicHolder = new G4LogicalVolume(solidHolder, matAl,
			solidHolder->GetName());

	G4LogicalVolume* logicBacking = new G4LogicalVolume(solidBacking, matTa,
			solidBacking->GetName());

	G4LogicalVolume* logicScrewBacking = new G4LogicalVolume(solidScrewBacking, matSteel,
			solidScrewBacking->GetName());

	G4LogicalVolume* logicWater = new G4LogicalVolume(solidWater, matWater,
			solidWater->GetName());

	G4LogicalVolume* logicWaterScrews = new G4LogicalVolume(solidWaterScrews, matBrass,
			solidWaterScrews->GetName());

	G4LogicalVolume* logicScrew = new G4LogicalVolume(solidScrew, matSteel,
			solidScrew->GetName());

	G4LogicalVolume* logicWaterTubes = new G4LogicalVolume(waterTubes, matCs,
			waterTubes->GetName());

	G4LogicalVolume* logicWaterTubesWater = new G4LogicalVolume(waterTubesWater, matWater,
			waterTubesWater->GetName());
	
	G4LogicalVolume* logicWaterTubesWaterInner = new G4LogicalVolume(waterTubesWaterInner, matWater,
			waterTubesWaterInner->GetName());

	/// Visualization attributes
	logicHolder->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
	logicScrew->SetVisAttributes(G4VisAttributes(G4Colour::Black()));
	logicBacking->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));
	logicScrewBacking->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));
	logicWater->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
	logicWaterTubes->SetVisAttributes(G4VisAttributes(G4Colour::White()));
	logicWaterScrews->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
	logicWaterTubesWater->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
	logicWaterTubesWaterInner->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));

	/// Place as Physical Volumes

	const G4double zOffset = -0.0 * mm;

	const G4double screwOffset = 3.42 * mm;

	const G4double graphiteSupportZ = -lengthTargetGraph - 3.17 * mm;
	const G4double graphiteSupportX = 12.5 * mm;
	const G4double graphiteSupportY = 0 * mm;

	if( fTarget == targetGraphite ){
		G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTargetC, matC,
				solidTargetC->GetName());
		G4LogicalVolume* logicGraphiteSupportH = new G4LogicalVolume(solidGraphiteSupportH, matSteel, 
				solidGraphiteSupportH->GetName());
		G4LogicalVolume* logicGraphiteSupportV = new G4LogicalVolume(solidGraphiteSupportV, matSteel, 
				solidGraphiteSupportV->GetName());
		logicTarget->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
		logicGraphiteSupportH->SetVisAttributes(G4VisAttributes(G4Colour::White( )));
		logicGraphiteSupportV->SetVisAttributes(G4VisAttributes(G4Colour::White( )));
		PlaceVolume(logicTarget, GetMotherVolume(),
				    -G4ThreeVector(0.,0.,lengthHolderHole + zOffset));
		PlaceVolume(logicGraphiteSupportH, GetMotherVolume(),
					-G4ThreeVector(graphiteSupportX, graphiteSupportY, graphiteSupportZ));
		PlaceVolume(logicGraphiteSupportH, GetMotherVolume(),
					-G4ThreeVector(-graphiteSupportX, -graphiteSupportY, graphiteSupportZ));
		PlaceVolume(logicGraphiteSupportV, GetMotherVolume(),
					-G4ThreeVector(graphiteSupportX + 0.8 * mm, graphiteSupportY, graphiteSupportZ - 3.21 * mm ));
		PlaceVolume(logicGraphiteSupportV, GetMotherVolume(),
					-G4ThreeVector(-graphiteSupportX - 0.8 * mm, graphiteSupportY, graphiteSupportZ - 3.21 * mm ));
	}
	if( fTarget == targetSource ){
		G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTargetCs, matCs,
				solidTargetCs->GetName());
		logicTarget->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
		PlaceVolume(logicTarget, GetMotherVolume(),
				    -G4ThreeVector(0.,0.,lengthHolderHole + zOffset));
	}

	PlaceVolume(logicHolder, GetMotherVolume(),
			-G4ThreeVector(0.,0.,lengthHolderHole + zOffset));

	// Placing all the M2.5 Screws
	for( int i = 0; i < 8; ++i ){
		PlaceVolume(logicScrew, GetMotherVolume(),
					-G4ThreeVector(centerM2Holes*std::sin(i*phiM2Holes), 
						       centerM2Holes*std::cos(i*phiM2Holes),
						       lengthHolderHole + zOffset + lenghtM2Backing + lengthBacking + screwOffset ));
	};

	PlaceVolume(logicBacking, GetMotherVolume(),
			-G4ThreeVector(0.,0.,lengthHolderHole + zOffset));

	PlaceVolume(logicScrewBacking, GetMotherVolume(),
			-G4ThreeVector(0.,0.,lengthHolderHole + zOffset));

	PlaceVolume(logicWater, GetMotherVolume(),
			-G4ThreeVector(0.,0.,lengthHolderHole + zOffset));
/*
	PlaceVolume(logicWaterTubes, GetMotherVolume(),
			-G4ThreeVector(centerWaterHoles,0.,lengthHolderHole + zOffset - lengthWaterTubes - lengthHolderBack));

	PlaceVolume(logicWaterScrews, GetMotherVolume(),
			-G4ThreeVector(centerWaterHoles,0.,zOffset + lengthHolderHole + tolerance));

	PlaceVolume(logicWaterScrews, GetMotherVolume(),
			-G4ThreeVector(-centerWaterHoles,0.,zOffset + lengthHolderHole + tolerance ));

	PlaceVolume(logicWaterTubes, GetMotherVolume(),
			-G4ThreeVector(-centerWaterHoles,0.,lengthHolderHole + zOffset - lengthWaterTubes - lengthHolderBack));
*/

	PlaceVolume(logicWaterTubesWater, GetMotherVolume(),
			-G4ThreeVector(-(rOuterHolderTop - 6)*std::sin(0.5*45.*deg), (rOuterHolderTop - 6)*std::cos(0.5*45.*deg), -centerWaterHoles + lengthHolderHole), xRot);

	PlaceVolume(logicWaterTubesWater, GetMotherVolume(),
			-G4ThreeVector(( rOuterHolderTop - 6 )*std::sin(0.5*45.*deg), -( rOuterHolderTop - 6 )*std::cos(0.5*45.*deg), -centerWaterHoles + lengthHolderHole), xRot);

	PlaceVolume(logicWaterTubesWaterInner, GetMotherVolume(),
			-G4ThreeVector((rOuterHolderTop - 12 - 5.41 )*std::sin(0.5*45.*deg), -( rOuterHolderTop - 12 - 5.41 )*std::cos(0.5*45.*deg), -centerWaterHoles + lengthHolderHole), xRot);

    PlaceVolume(logicWaterTubesWaterInner, GetMotherVolume(),
			-G4ThreeVector(-( rOuterHolderTop - 12 - 5.41 )*std::sin(0.5*45.*deg), ( rOuterHolderTop - 12 - 5.41 )*std::cos(0.5*45.*deg), -centerWaterHoles + lengthHolderHole), xRot);

	return nullptr;
}
