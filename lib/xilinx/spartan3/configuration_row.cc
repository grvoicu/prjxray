#include <prjxray/xilinx/spartan3/configuration_row.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

bool Row::IsValidFrameAddress(FrameAddress address) const {
  auto addr_bus = configuration_buses_.find(address.block_type());
  if (addr_bus == configuration_buses_.end())
    return false;
  return addr_bus->second.IsValidFrameAddress(address);
}

absl::optional<FrameAddress>
Row::GetNextFrameAddress(FrameAddress address) const {
  // Find the bus for the current address.
  auto addr_bus = configuration_buses_.find(address.block_type());

  // If the current address isn't in a known bus, no way to know the next.
  if (addr_bus == configuration_buses_.end())
    return {};

  // Ask the bus for the next address.
  absl::optional<FrameAddress> next_address =
      addr_bus->second.GetNextFrameAddress(address);
  if (next_address)
    return next_address;

  // The current bus doesn't know what the next address is. Rows come next
  // in frame address numerical order so punt back to the caller to figure
  // it out.
  return {};
}

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

namespace spartan3 = prjxray::xilinx::spartan3;

namespace YAML {

Node convert<spartan3::Row>::encode(const spartan3::Row &rhs) {
  Node node;
  node.SetTag("xilinx/spartan3/row");
  node["configuration_buses"] = rhs.configuration_buses_;
  return node;
}

bool convert<spartan3::Row>::decode(const Node &node, spartan3::Row &lhs) {
  if (!node.Tag().empty() && node.Tag() != "xilinx/spartan3/row") {
    return false;
  }

  lhs.configuration_buses_ =
      node["configuration_buses"]
          .as<std::map<spartan3::BlockType, spartan3::ConfigurationBus>>();
  return true;
}

} // namespace YAML
