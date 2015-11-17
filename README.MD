### Mapsets informations

 - MapSet name : Nemrod Transparent Trails (Family ID: 101)
   - Nemrod Camp map ID: 		10000000  
   - Nemrod St-Hilaire map ID: 	10000001
   
 - MapSet name : Nemrod Complete (Family ID: 102)
   - Nemrod Camp map ID: 		10000000
   


### Directory Structure

./bin: Binaries from our homemade tools  
./Installer: Generated installers  
./MappingData: Source data used to generated tracks and POIs  
./NemrodToolkit: Homemade tools source code  
./Output-MapTK: Output generated via MapTK  
./Tools: Free to use tools used by the project  


### MapTK Project

To generate maps using MapTK, use the MapTK.prj file.

###### Map files

Maps use the MP format. Refer to MapTK documentation for the file structure, headers, features etc. MP files are edited with GPSMapEdit, in the tools folder. 
Documentation from cGpsMapper can also be really usefull, there is the PDF in the tools folder.
  
One exception, is that contrary to what is specified, the 0x4b polygon (Area of Map coverage) for indidual maps does not have to be in last level (inactive), which is not displayed in GPSMapedit.  
It is only used for routing, and is ignored by MapTK overview map generator regardless of its level but will be compiled in IMG if on level 0, so basically it needs to be on any level other than 0. Note that Nemrod maps do not do routing, and probly never will.

###### Custom Types

Custom types are declared in the MapTK.prj file. To use them in GPSMapEdit use map skins (View Menu). 

 - 0x10F00 : Lot lines
 
###### Default altered Types
 
 - 0x0e : Landing Strip
 - 0x14 : Forest
 - 0x40 : Lake
 - 0x49 : River
 - 0x51 : Humid

###### Generation
 
 - Run the compile-maptk.bat file which will: 
   - Generate overview map
   - Compile overview
   - Compile maps using maptk prj files
   - Generate the MSI from WiX data
 
### Installing to MapSource/Basecamp on Windows

Use the MSI from the Generation section to install.

###### Manual install

Once the project is generated, a registry file is placed in the output. It needs to be modified to suit your need. Do not forget that
Mapsource/Basecamp are x86 executables, so on a x64 Windows, the entries needs to be in the Wow6432Node.  
  
If you want to save this registry info, its a good time to back it up since MapTK will overwrite it sometimes. (seems to happen randomly)  
