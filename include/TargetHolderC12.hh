#ifndef TargetHolderC12_hh
#define TargetHolderC12_hh

#include "GeometryObject.hh"

class G4UIcmdWithAString;

#include <memory>
using std::shared_ptr;

class TargetHolderC12 : public GeometryObject {

    public:
        TargetHolderC12();
        virtual ~TargetHolderC12();

        G4VPhysicalVolume *Construct();
        void ConstructSDandField() {};

        virtual void SetNewValue(G4UIcommand* command, G4String value);

    private:
        shared_ptr<G4UIcmdWithAString> fTargetCmd;

        bool fTargetChosen = false;
        enum {targetGraphite, targetSource, targetEvaporated} fTarget;
};

#endif
