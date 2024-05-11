#ifndef TargetChamberC12_hh
#define TargetChamberC12_hh

#include "GeometryObject.hh"

class TargetChamberC12 : public GeometryObject {

    public:
        TargetChamberC12();
        virtual ~TargetChamberC12();

        G4VPhysicalVolume *Construct();
        void ConstructSDandField() {};

        virtual void SetNewValue(G4UIcommand* command, G4String value);

    private:
};

#endif
