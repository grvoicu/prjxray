#include <prjxray/xilinx/spartan3/block_type.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

std::ostream &operator<<(std::ostream &o, BlockType value) {
  switch (value) {
  case BlockType::TERM_IOI_CLB_GCLK:
    o << "TERM/IOI/CLB/GCLK";
    break;
  case BlockType::BLOCK_RAM:
    o << "Block RAM";
    break;
  case BlockType::BLOCK_RAM_INT:
    o << "Block RAM Interconnect";
    break;
  }

  return o;
}

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

namespace YAML {

Node convert<prjxray::xilinx::spartan3::BlockType>::encode(
    const prjxray::xilinx::spartan3::BlockType &rhs) {
  switch (rhs) {
  case prjxray::xilinx::spartan3::BlockType::TERM_IOI_CLB_GCLK:
    return Node("TERM_IOI_CLB_GCLK");
  case prjxray::xilinx::spartan3::BlockType::BLOCK_RAM:
    return Node("BLOCK_RAM");
  case prjxray::xilinx::spartan3::BlockType::BLOCK_RAM_INT:
    return Node("BLOCK_RAM_INT");
  default:
    return Node(static_cast<unsigned int>(rhs));
  }
}

bool YAML::convert<prjxray::xilinx::spartan3::BlockType>::decode(
    const Node &node, prjxray::xilinx::spartan3::BlockType &lhs) {
  auto type_str = node.as<std::string>();

  if (type_str == "TERM_IOI_CLB_GCLK") {
    lhs = prjxray::xilinx::spartan3::BlockType::TERM_IOI_CLB_GCLK;
    return true;
  } else if (type_str == "BLOCK_RAM") {
    lhs = prjxray::xilinx::spartan3::BlockType::BLOCK_RAM;
    return true;
  } else if (type_str == "BLOCK_RAM_INT") {
    lhs = prjxray::xilinx::spartan3::BlockType::BLOCK_RAM_INT;
    return true;
  } else {
    return false;
  }
}

} // namespace YAML
