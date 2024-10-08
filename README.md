# Can-am Blinker Controller (STM32) 
The source contained within this repository is intended for the Can-am Blinker Controller.  

## Architecture
Bare metal -- application written in C.       

## Dependencies
* STM32Cube IDE: v1.16.0
   

## Tagged Versions 
* v0.0.1 -- Compiles. The health LED will blink.  There is no CLI interface due to compilation errors.  

* v0.0.2 -- Some files didn't appear to be properly tracked.  Compiles. The health LED will blink.  There is no CLI interface due to compilation errors.

* v0.0.3 -- CLI stuff was added back in and the solution compiles.  To obviate compilation errors,console functions were added to uart files (pulled from console files).

* v0.0.4 -- A good amount of blinker logic has been added in and the source compiles.  

* v0.0.5 -- Debug menu items were added and the code compiles.  

* v0.0.6 -- Support for the watchdog has been added in.  The dog is kicked in many places within the menu.  This code compiles in STM32 Cube IDE.

* v0.0.7 -- A handful of bugs were addressed and the algorithm was well tested.  This version is very close to a release candidate.  

* v0.1.0 -- Fixed issue where watchdog was tripping too early.  The serial BAUD rate was changed back to 57600.  
