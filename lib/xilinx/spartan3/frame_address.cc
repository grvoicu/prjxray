#include <iomanip>

#include <prjxray/bit_ops.h>
#include <prjxray/xilinx/spartan3/frame_address.h>

namespace prjxray {
namespace xilinx {
namespace spartan3 {

// According to XAPP452 pg. 3
FrameAddress::FrameAddress(spartan3::BlockType block_type, uint8_t row,
                           uint8_t column, uint8_t minor) {
  address_ = bit_field_set(0, 27, 25, block_type);
  address_ = bit_field_set(address_, 24, 17, column);  
  address_ = bit_field_set(address_, 16, 9, minor);
  // address_ = bit_field_set(address_, 8, 0, row);
}

bool FrameAddress::is_bottom_half_rows() const { return false; }

spartan3::BlockType FrameAddress::block_type() const {
  return static_cast<typename spartan3::BlockType>(
      bit_field_get(address_, 27, 25));
}

uint8_t FrameAddress::row() const { return 0; }

uint8_t FrameAddress::column() const { return bit_field_get(address_, 24, 17); }

uint8_t FrameAddress::minor() const { return bit_field_get(address_, 16, 9); }

std::ostream &operator<<(std::ostream &o, const FrameAddress &addr) {
  o << "[" << std::hex << std::showbase << std::setw(10)
    << static_cast<uint32_t>(addr) << "] " 
    << " Row=" << std::setw(2) << std::dec 
    << static_cast<unsigned int>(addr.row()) << "Column =" << std::setw(2)
    << std::dec << addr.column() << " Minor=" << std::setw(2) << std::dec
    << static_cast<unsigned int>(addr.minor()) << " Type=" << addr.block_type();
  return o;
}

} // namespace spartan3
} // namespace xilinx
} // namespace prjxray

namespace YAML {

namespace spartan3 = prjxray::xilinx::spartan3;

Node convert<spartan3::FrameAddress>::encode(
    const spartan3::FrameAddress &rhs) {
  Node node;
  node.SetTag("xilinx/spartan3/frame_address");
  node["block_type"] = rhs.block_type();
  node["row"] = static_cast<unsigned int>(rhs.row());
  node["column"] = static_cast<unsigned int>(rhs.column());
  node["minor"] = static_cast<unsigned int>(rhs.minor());
  return node;
}

bool convert<spartan3::FrameAddress>::decode(const Node &node,
                                             spartan3::FrameAddress &lhs) {
  if (!(node.Tag() == "xilinx/spartan3/frame_address" ||
        node.Tag() == "xilinx/spartan3/configuration_frame_address") ||
      !node["block_type"] || !node["row"] || !node["column"] || !node["minor"])
    return false;

  lhs = spartan3::FrameAddress(node["block_type"].as<spartan3::BlockType>(),
                               node["row"].as<unsigned int>(),
                               node["column"].as<unsigned int>(),
                               node["minor"].as<unsigned int>());
  return true;
}

} // namespace YAML
