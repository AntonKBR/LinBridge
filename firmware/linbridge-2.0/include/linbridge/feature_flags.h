#pragma once

namespace linbridge::features {

// Runtime features stay disabled until their dedicated migration step supplies
// an evidence-backed configuration, validation and tests.
inline constexpr bool kLin = false;
inline constexpr bool kMfaOutput = false;
inline constexpr bool kCcsOutput = false;
inline constexpr bool kKeyOutput = false;
inline constexpr bool kCan = false;
inline constexpr bool kAutomaticIllumination = false;
inline constexpr bool kWifiDiagnostics = false;

}  // namespace linbridge::features
