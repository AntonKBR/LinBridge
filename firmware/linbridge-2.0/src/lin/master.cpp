#include "linbridge/lin/master.h"

#include "linbridge/lin/protocol.h"

namespace linbridge::lin {

void Master::begin() { transport_.begin(); }

void Master::sendFixedIllumination() {
    transport_.sendFrame(kIlluminationIdentifier,
                         kFixedIlluminationPayload.data(),
                         kFixedIlluminationPayload.size());
}

void Master::requestButtons() {
    transport_.sendHeader(kButtonIdentifier);
}

void Master::requestAcc() { transport_.sendHeader(kAccIdentifier); }

}  // namespace linbridge::lin
