#include <prjxray/xilinx/spartan3/global_clock_region.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

bool GlobalClockRegion::IsValidFrameAddress(FrameAddress address) const {
  auto addr_row = rows_.find(address.row());
  if (addr_row == rows_.end())
    return false;
  return addr_row->second.IsValidFrameAddress(address);
}

absl::optional<FrameAddress>
GlobalClockRegion::GetNextFrameAddress(FrameAddress address) const {
  // Find the row for the current address.
  auto addr_row = rows_.find(address.row());

  // If the current address isn't in a known row, no way to know the next.
  if (addr_row == rows_.end())
    return {};

  // Ask the row for the next address.
  absl::optional<FrameAddress> next_address =
      addr_row->second.GetNextFrameAddress(address);
  if (next_address)
    return next_address;

  // The current row doesn't know what the next address is.  Assume that
  // the next valid address is the beginning of the next row.
  if (++addr_row != rows_.end()) {
    auto next_address =
        FrameAddress(address.block_type(), addr_row->first, 0, 0);
    if (addr_row->second.IsValidFrameAddress(next_address))
      return next_address;
  }

  // Must be in a different global clock region.
  return {};
}

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

namespace spartan3 = prjxray::xilinx::spartan3;

namespace YAML {

Node convert<spartan3::GlobalClockRegion>::encode(
    const spartan3::GlobalClockRegion &rhs) {
  Node node;
  node.SetTag("xilinx/spartan3/global_clock_region");
  node["rows"] = rhs.rows_;
  return node;
}

bool convert<spartan3::GlobalClockRegion>::decode(
    const Node &node, spartan3::GlobalClockRegion &lhs) {
  if (!node.Tag().empty() &&
      node.Tag() != "xilinx/spartan3/global_clock_region") {
    return false;
  }

  lhs.rows_ = node["rows"].as<std::map<unsigned int, spartan3::Row>>();
  return true;
}

} // namespace YAML
