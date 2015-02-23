### Mapset informations

MapSet name : Nemrod Maps  
Family ID: 101  

Nemrod Camp map ID: 		10000000  
Nemrod St-Hilaire map ID: 	10000001  


### Directory Structure

./Installer: Generated installers  
./MappingData: Source data used to generated tracks and POIs  
./Output-MapTK: Output generated via MapTK  
./Tools: Free to use tools used by the project  


### MapTK Project

To generate maps using MapTK, use the MapTK.prj file.

###### Map files

Maps use the MP format. Refer to MapTK documentation for the file structure, headers, features etc. MP files are edited with GPSMapEdit, in the tools folder.  
  
One exception, is that contrary to what is specified, the 0x4b polygon (Area of Map coverage) for indidual maps does not have to be in last level, which is not displayed in GPSMapedit.  
It is only used for routing, and is ignored by MapTK regardless of its level. Note that Nemrod maps do not do routing, and probly never will.
  
When our own overview map generator is complete, we will also use these to realize 0x4a (Area of Map selection) using these, and use their extent to add the 0x4b polygon to the overview map.


###### Manual generation
 
 - Create Overview map (./nemrod.mp is created from the maps)
 - Open ./nemrod.mp in GPSMapEdit and in File>Map Properties, give it a significant name (ie: Nemrod Maps)
 - Run MAKE
 
 
###### Automatic generation

Automatic generation should not be used until scripts are fixed, not really usefull anymore as only the name needs to be changed.  
~~Using automatic generation calls a script that sets the levels and rename the map~~  
  
 - ~~Create Overview map (./nemrod.mp is created)~~
 - ~~Run the script (click script button) on the ./nemrod.mp file~~
 - ~~Run MAKE~~

 
### Installing to MapSource/Basecamp on Windows

Once the project is generated, a registry file is placed in the output. It needs to be modified to suit your need. Do not forget that
Mapsource/Basecamp are x86 executables, so on a x64 Windows, the entries needs to be in the Wow6432Node.  
  
If you want to save this registry info, its a good time to back it up since MapTK will overwrite it sometimes. (seems to happen randomly)  