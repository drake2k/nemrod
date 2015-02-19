# Mapset informations

MapSet name : Nemrod Maps
Family ID: 101

Nemrod Camp map ID: 		10000000
Nemrod St-Hilaire map ID: 	10000001


# Directory Structure

./Installer: Generated installers
./Output-MapTK: Outpout generated via MapTK
./Tools: Free to use tools used by the project


# MapTK Project

To generate maps using MapTK, use the MapTK.prj file.

### Manual generation
 
 - Create Overview map (./nemrod.mp is created from the maps)
 - Open ./nemrod.mp in GPSMapEdit and in File>Map Properties, give it a significant name (ie: Nemrod Maps)
 - Change the level 0 in the overview map to match level 0 in maps it was created from.
 - DO NOT CHANGE ANYTHING ELSE
 - Run MAKE
 
 
### Automatic generation
Using automatic generation calls a script that sets the levels and rename the map  
  
 - Create Overview map (./nemrod.mp is created)
 - Run the script (click script button) on the ./nemrod.mp file
 - Run MAKE
