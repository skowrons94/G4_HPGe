#ifndef ColdTrap_hh
#define ColdTrap_hh

#include "GeometryObject.hh"

class ColdTrap : public GeometryObject {

    public:
        ColdTrap();
        virtual ~ColdTrap();

        G4VPhysicalVolume *Construct();
        void ConstructSDandField() {};

        virtual void SetNewValue(G4UIcommand* command, G4String value);

    private:
};

#endif
