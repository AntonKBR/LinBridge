#include <Arduino.h>

#include "linbridge/board_config.h"
#include "linbridge/feature_flags.h"

static_assert(!linbridge::features::kLin);
static_assert(!linbridge::features::kMfaOutput);
static_assert(!linbridge::features::kCcsOutput);
static_assert(!linbridge::features::kKeyOutput);
static_assert(!linbridge::features::kCan);
static_assert(!linbridge::features::kAutomaticIllumination);
static_assert(!linbridge::features::kWifiDiagnostics);

void setup() {}

void loop() {}
