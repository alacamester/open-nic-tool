# open-nic-tool
This tool is intended to configure onic firmware-extensions. (needs open-nic-driver from alacamester)
- extension 1: simple hardware-aided packet filter module
  * the tool sends filter initialization data to the onic-shell. Only packets passing the filter are received.

##Directory structure: 
- tool : contains onic_tool
- script : contains scripts to bring up onic, configure hw-filters, etc.
- udp_send : socket-based test app, to send UDP packets

##USAGE:
 onic_tool <device_name> <option> <values>
 * Options: 
 -  r <register> : read register address (hex)
 -  w <register> <value> : write value to register address(hex)
 -  f <filename> : initialize filter with csv file content
 -  c : clear (all) filters, drop all packets

CSV file format:
<P/D>, <Src_IP/Netmask>, <Dst_IP/Netmask>, <IP_protocol>, <Src_Port>, <Dst_port>
- P/D : pass or drop filter-rule
- example: 
  P, 10.0.0.112, 10.0.0.0/16, 17, 0, 0
  (pass UDP packets from 10.0.0.112 to 10.0.0.0/16 with any port value)

##TODO:
 - add filter statistics-readout
 - implement A/B interface filtering (for 2 direction links)
