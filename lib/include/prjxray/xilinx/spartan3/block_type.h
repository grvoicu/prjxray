#ifndef PRJXRAY_LIB_XILINX_SPARTAN3_BLOCK_TYPE_H_
#define PRJXRAY_LIB_XILINX_SPARTAN3_BLOCK_TYPE_H_

#include <ostream>

#include <yaml-cpp/yaml.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

// According to XAPP452 pg. 12 there are 3 types of configuration frames:
// Type 0: input/output electrical standard (TERM), input/output interconnect (IOI), CLB, clocking
// Type 1: Block RAM
// Type 2: Block RAM interconnect
enum class BlockType : unsigned int {
  TERM_IOI_CLB_GCLK = 0x0,
  BLOCK_RAM = 0x1,
  BLOCK_RAM_INT = 0x2,
};

std::ostream &operator<<(std::ostream &o, BlockType value);

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

namespace YAML {
template <> struct convert<prjxray::xilinx::spartan3::BlockType> {
  static Node encode(const prjxray::xilinx::spartan3::BlockType &rhs);
  static bool decode(const Node &node,
                     prjxray::xilinx::spartan3::BlockType &lhs);
};
} // namespace YAML

#endif // PRJXRAY_LIB_XILINX_SPARTAN3_BLOCK_TYPE_H_
