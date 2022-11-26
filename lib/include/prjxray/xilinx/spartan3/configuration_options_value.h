#ifndef PRJXRAY_LIB_XILINX_SPARTAN3_CONFIGURATION_OPTIONS_VALUE_H
#define PRJXRAY_LIB_XILINX_SPARTAN3_CONFIGURATION_OPTIONS_VALUE_H

#include <prjxray/bit_ops.h>
#include <prjxray/xilinx/configuration_packet.h>
#include <prjxray/xilinx/configuration_register.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

class ConfigurationOptionsValue {
public:
  enum class StartupClockSource : uint32_t {
    CCLK = 0x0,
    User = 0x1,
    JTAG = 0x2,
  };

  enum class CclkFrequency : uint32_t {
    MHz_3   = 0x4,
    MHz_6   = 0x0,
    MHz_12  = 0x1,
    MHz_25  = 0x2,
    MHz_50  = 0x3,
    MHz_100 = 0x6,
  };

  enum class SignalReleaseCycle : uint32_t {
    Phase1 = 0x0,
    Phase2 = 0x1,
    Phase3 = 0x2,
    Phase4 = 0x3,
    Phase5 = 0x4,
    Phase6 = 0x5,
    Keep = 0x7,
  };

  enum class StallCycle : uint32_t {
    Phase0 = 0x0,
    Phase1 = 0x1,
    Phase2 = 0x2,
    Phase3 = 0x3,
    Phase4 = 0x4,
    Phase5 = 0x5,
    Phase6 = 0x6,
    NoWait = 0x7,
  };

  ConfigurationOptionsValue() : value_(0) {}

  operator uint32_t() const { return value_; }

  ConfigurationOptionsValue &SetCrcBypass(bool enabled) {
    value_ = bit_field_set(value_, 29, 29, enabled ? 1 : 0);
    return *this;
  }

  ConfigurationOptionsValue &SetAddPipelineStageForDoneIn(bool enabled) {
    value_ = bit_field_set(value_, 25, 25, enabled ? 1 : 0);
    return *this;
  }

  ConfigurationOptionsValue &SetDriveDoneHigh(bool enabled) {
    value_ = bit_field_set(value_, 24, 24, enabled);
    return *this;
  }

  ConfigurationOptionsValue &SetReadbackIsSingleShot(bool enabled) {
    value_ = bit_field_set(value_, 23, 23, enabled);
    return *this;
  }

  ConfigurationOptionsValue &SetCclkFrequency(CclkFrequency frequency) {
    value_ = bit_field_set(value_, 22, 19, static_cast<uint32_t>(frequency));
    return *this;
  }

  ConfigurationOptionsValue &SetStartupClockSource(StartupClockSource source) {
    value_ = bit_field_set(value_, 16, 15, static_cast<uint32_t>(source));
    return *this;
  }

  ConfigurationOptionsValue &
  SetReleaseDonePinAtStartupCycle(SignalReleaseCycle cycle) {
    value_ = bit_field_set(value_, 14, 12, static_cast<uint32_t>(cycle));
    return *this;
  }

  ConfigurationOptionsValue &
  SetStallAtStartupCycleUntilDciMatch(StallCycle cycle) {
    value_ = bit_field_set(value_, 11, 9, static_cast<uint32_t>(cycle));
    return *this;
  };

  ConfigurationOptionsValue &
  SetStallAtStartupCycleUntilMmcmLock(StallCycle cycle) {
    value_ = bit_field_set(value_, 8, 6, static_cast<uint32_t>(cycle));
    return *this;
  };

  ConfigurationOptionsValue &
  SetReleaseGtsSignalAtStartupCycle(SignalReleaseCycle cycle) {
    value_ = bit_field_set(value_, 5, 3, static_cast<uint32_t>(cycle));
    return *this;
  }

  ConfigurationOptionsValue &
  SetReleaseGweSignalAtStartupCycle(SignalReleaseCycle cycle) {
    value_ = bit_field_set(value_, 2, 0, static_cast<uint32_t>(cycle));
    return *this;
  }

private:
  uint32_t value_;
}; // namespace spartan3

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

#endif // PRJXRAY_LIB_XILINX_SPARTAN3_CONFIGURATION_OPTIONS_VALUE_H
