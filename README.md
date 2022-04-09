# open-nic-tool
This tool is intended to configure onic firmware-extensions.
- extension 1: simple hardware-aided packet filter module
  the tool sends filter initialization data to the onic-shell. Only packets passing the filter are received.

Directory structure: 
- tool : contains onic_tool
- script : contains scripts to bring up onic, configure hw-filters, etc.
- udp_send : socket-based test app, to send UDP packets

TODO:
 - add config support for reading filter settings
 - add filter statistics-readout
