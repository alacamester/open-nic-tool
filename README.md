# open-nic-tool

This tool is intended to configure onic firmware-extensions. (needs open-nic-driver from alacamester)
- extension 1: simple hardware-aided packet filter module
  * the tool sends filter initialization data to the onic-shell. Only packets passing the filter are received.

## Directory structure: 
- tool : contains onic_tool
- script : contains scripts to bring up onic, configure hw-filters, etc.
- udp_send : socket-based test app, to send UDP packets

## USAGE:
 onic_tool <device_name> <option> <values>
 * Options: 
 -  r <register> : read register address (hex)
 -  w <register> <value> : write value to register address(hex)
 -  f <filename> : initialize filter with csv file content
 -  c : clear (all) filters, drop all packets

CSV file format:
<[0..4]>, <Pass/Drop>, <Src_IP/Netmask>, <Dst_IP/Netmask>, <IP_protocol>, <Src_Port>, <Dst_port>
- [0..4] : Upload to
 * 0 = Iface A only
 * 1 = Iface B only
 * 2 = Iface A & B
 * 3 = Iface A & swap IP+port for B
 * 4 = swap IP+port for A & B
- Pass/Drop : pass or drop filter-rule
- example: 
  1, Pass, 10.0.0.112, 10.0.0.0/16, 17, 0, 0
  (pass UDP packets from 10.0.0.112 to 10.0.0.0/16 with any port value on interface B)
  2, Pass, fe80::9085:91b1:7843:71b2, ff02::1:0/120, 17, 546, 547 
  (pass UDP packets between IPv6 addresses, and ports)

## TODO:
 - add filter statistics-readout
