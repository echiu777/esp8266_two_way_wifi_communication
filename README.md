# esp8266_two_way_wifi_communication


Files:
  get_MAC.ino   (first burn this to get MAC address)
  talk_with.ino (then modify MACs and burn this to boards)

Schematic:
```
5V                                       
 │                                       
 │                                       
R=10K                                    
 │                                       
 │                                       
 │       ┌─────────┐                     
 ├─────D5│         │D0────R=330───LED──┐ 
 │       │ ESP8266 │                   │ 
SW       │         │D1────R=330───LED──┤ 
 │       └─────────┘                   │ 
 │                                     │ 
GND                                   GND
```

![alt text](https://github.com/echiu777/esp8266_two_way_wifi_communication/blob/main/DSCF8002.jpg)
