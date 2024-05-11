#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class HPGeDetector;
class TargetHolderC12;
class TargetChamberC12;
class ColdTrap;


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4LogicalVolume* GetScoringVolume() const
    {
        return m_scoringVolume;
    }

protected:
    HPGeDetector *m_hpgeDetector = nullptr;
    TargetHolderC12 *m_targetHolder = nullptr;
    TargetChamberC12 *m_targetChamber = nullptr;
    ColdTrap *m_coldTrap = nullptr;

    G4LogicalVolume* m_scoringVolume = nullptr;
};

#endif // #ifndef DetectorConstruction_hh

