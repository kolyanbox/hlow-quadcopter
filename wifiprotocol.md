the quadcopter will only recognise unsigned chars )numbers from 0 -255. baudrate is 9600.
No parity bit will be send and only 1 stop bit.

When sending messages to the quadcopter it atleast has the common message. every message will end with a 0x0a and/ or 0x0d (\n\r)


common message:
| msb | | | lsb |
|:----|:|:|:----|
| 1 bit | 4 bits | 2 bits | 1 bit|
| send/ receive | type | payload | extend |

| send/ receive | bit is 1 when sending a command 0 when asking for telemetry data |
|:--------------|:-----------------------------------------------------------------|
| type          | defines the command type. a maximum of 16 different type of telemetry data can be retrieved and 16 different commands can be send |
| payload       | the payload of the message                                       |
| extend        | when set to 1 the message will be extended with 1 byte. 0 when it won't be extended |

Extended message:
| msb | lsb |
|:----|:----|
| 6 bits | 2 bits |
| extended payload | extend |

| extended payload | the extended payload |
|:-----------------|:---------------------|
| extend           | defines the amount of bytes added with payload. When set to 11 there will 2 bytes with payload send and 1 extra extended message. |

message types:
Sending commands:
| 0000 | initialisation |
|:-----|:---------------|
| 0001 | kill task      |
| 0010 | get running tasks  information |
| 0011 |                |
| 0100 |                |
| 0101 |                |
| 0110 |                |
| 0111 |                |
| 1000 |                |
| 1001 |                |
| 1010 |                |
| 1011 |                |
| 1100 |                |
| 1101 |                |
| 1110 |                |
| 1111 | emergency stop |

Requesting telemetry data:
| 0000 | distance to ground |
|:-----|:-------------------|
| 0001 | battery status     |
| 0010 | angle              |
| 0011 | speed              |
| 0100 | gps position       |
| 0101 | compass            |
| 0110 |                    |
| 0111 |                    |
| 1000 |                    |
| 1001 |                    |
| 1010 |                    |
| 1011 |                    |
| 1100 |                    |
| 1101 |                    |
| 1110 |                    |
| 1111 | emergency stop     |

message examples:
| 1 | 0001 | 11 | 1 | 111111 | 00 |
|:--|:-----|:---|:--|:-------|:---|
| send a command | kill a task | first part of payload (11) | message is extended | second part of payload (111111) | message don't have any more bytes |

Payload:
| command | payload | description |
|:--------|:--------|:------------|
| initialisation | 11111111 | When a bit is 1 it means that the sensor should be initialised. 0 means that is shouldn't be initialised. bit 1 (msb): initialise distance to ground sensor, bit 2: initialise speed sendor, bit 3: initialise angle sensor, bit 4: initialise compass, bit 5: initialise gps bit 6: start motor at startup, bit 7/8 when 00 start motors for 1 second when 01 start motors for 5 seconds when 10 start motor for 10 seconds when 11 start motors for 15 seconds |
| kill task | 11111111 | kills the task which has the given id |
| get running tasks information | xx      | No payload required |
| emergency stop | xx      | No payload required |

| request telemetry | payload | description |
|:------------------|:--------|:------------|
| distance to ground | 11      | 00 means that the distance from take off point to the current height will be requested. 01 means that the current height to the ground will be returned |
| battery status    | xx      | no payload required |
| angle             | 11      | when 00 request angle of axle x, when 01 request angle of axle y, when 10 request angle axle z |
| speed 11          | when 00 request speed of axle x, when 01 request speed of axle y, when 10 request speed axle z |
| gps position      | xx      | request gps position. No payload required |
| compass           | when 00 request compass value of axle x, when 01 request compass value of axle y, when 10 request compass value axle z |
| emergency stop    | xx      | No payload required |