#pragma once

#include "linbridge/lin/transport.h"

namespace linbridge::lin {

class Master {
  public:
    explicit Master(Transport& transport) : transport_(transport) {}

    void begin();
    void sendFixedIllumination();
    void requestButtons();
    void requestAcc();

  private:
    Transport& transport_;
};

}  // namespace linbridge::lin
