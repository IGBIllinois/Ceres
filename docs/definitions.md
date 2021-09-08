<a id="top"></a>

# Definitions

## C1

The industrial PLC responsible for the control of the four cable winches.  The tension in each of the cables determines the position and height of the sensor dolly. 

IP Address: 172.16.1.1

Ports:
	UDP 21501


## C2

The Windows 10 based PC running the control middleware and operating application.

IP Address: 172.16.144.144

Ports:
	TCP 10201
	TCP 10202
	TCP 10203  <-- Connects to C3 for remote control commands
	TCP 18253
	UDP 19203
	TCP 54759-54769


## C3

The Windows 10 based PC running the data aquisition and remote control applications.

IP Address: 172.16.1.3


## C4

The Windows 10 based PC running in the sensor dolly.

IP Address: 172.16.1.4
