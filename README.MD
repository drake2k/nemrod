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
It is only used for routing, and is ignored by MapTK overview map generator regardless of its level but will be compiled in IMG if on level 0, so basically it needs to be on any level other than 0. Note that Nemrod transparent maps do not do routing.

###### Custom Types

Custom types are declared in the MapTK.prj file. To use them in GPSMapEdit use map skins (View Menu). 

 - 0x10F00 : Lot lines
 
###### Default altered Types

They were altered because either the name was not convenient, didnt provide translation or was not supported correctly by MapSource/Basecamp
 
 - 0x0e : Landing Strip (was "airport runaway")
 - 0x14 : Forest (was "national park")
 - 0x40 : Lake (was "small lake")
 - 0x49 : River (was "small river")
 - 0x51 : Humid (was "wetland/swamp" and the icon was a grid which was heavy on receiver)

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

### Extracting data from maps

To get the data from a compiled map there are 2 cases. Either from an IMG file or from the files in the new garmin format.

###### Old format (img)

 - Try to open in GPSMapEdit
 - If locked, uses the unlocking tools in the tools directory to XOR and unlock maps
 - Then open in GPSMapEdit
 
###### New format (GMAP) 

 - Use GMapTools in the tools directory and join all the files for a map into an img file
 - Try to open in GPSMapEdit
 - If locked, uses the unlocking tools in the tools directory to XOR and unlock maps
 
###### Old and new format, alternative

 - Select all desired maps in map source, from many mapsets if needed
 - Send to Device, target harddrive (C:\ will send to C:\Garmin)
 - In GMapTools, select the GMASupp file and split to single maps
 - XOR and Unlock if needed
 
Using either of these techniques, opening the basemap img with GPSMapEdit first is usefull to get the IMG names that you want
