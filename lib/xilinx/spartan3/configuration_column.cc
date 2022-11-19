#include <prjxray/xilinx/spartan3/configuration_column.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

bool ConfigurationColumn::IsValidFrameAddress(FrameAddress address) const {
  return address.minor() < frame_count_;
}

absl::optional<FrameAddress>
ConfigurationColumn::GetNextFrameAddress(FrameAddress address) const {
  if (!IsValidFrameAddress(address))
    return {};

  if (static_cast<unsigned int>(address.minor() + 1) < frame_count_) {
    return address + 0x200; // the next minor address, bits 8:0 are always 0
  }

  // Next address is not in this column.
  return {};
}

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

namespace spartan3 = prjxray::xilinx::spartan3;

namespace YAML {

Node convert<spartan3::ConfigurationColumn>::encode(
    const spartan3::ConfigurationColumn &rhs) {
  Node node;
  node.SetTag("xilinx/spartan3/configuration_column");
  node["frame_count"] = rhs.frame_count_;
  return node;
}

bool convert<spartan3::ConfigurationColumn>::decode(
    const Node &node, spartan3::ConfigurationColumn &lhs) {
  if (!node.Tag().empty() &&
      node.Tag() != "xilinx/spartan3/configuration_column") {
    return false;
  }

  lhs.frame_count_ = node["frame_count"].as<unsigned int>();
  return true;
}

} // namespace YAML
