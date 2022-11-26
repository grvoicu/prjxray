#ifndef PRJXRAY_LIB_XILINX_SPARTAN3_COMMAND_H_
#define PRJXRAY_LIB_XILINX_SPARTAN3_COMMAND_H_

namespace prjxray {
namespace xilinx {
namespace spartan3 {

// Command register map according to XAPP452 pg. 4
enum class Command : uint32_t {
  NOP = 0x0,
  WCFG = 0x1,
  MFWR = 0x2,
  LFRM = 0x3,
  RCFG = 0x4,
  START = 0x5,
  RCAP = 0x6,
  RCRC = 0x7,
  AGHIGH = 0x8,
  SWITCH = 0x9,
  GRESTORE = 0xA,
  SHUTDOWN = 0xB,
  GCAPTURE = 0xC,
  DESYNC = 0xD,
};

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

#endif // PRJXRAY_LIB_XILINX_SPARTAN3_COMMAND_H_
