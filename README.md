# cad2shape
## Convert bulgarian CAD format (from Geodesy, Cartography and Cadastre Agency) to shapefile 
### Full featured project with Visual Studio 2013, C++,  GDAL OGR and boost/filesystem

How to start development:

Download the project cad2shape.

Download GDAL Development Kits from:

http://www.gisinternals.com/query.html?content=filelist&file=release-1800-x64-dev.zip

Download and compile boost for VS2013 64 bit.

### Open cad2shape.sln with VS 2013 and change paths from config screens to you custom paths

Config screen for GDAL
![Screenshot](https://github.com/aspetkov/cad2shape/blob/master/vcdirectories.PNG)

Config screen for Boost
![Screenshot](https://github.com/aspetkov/cad2shape/blob/master/boostdirectory.PNG)

![Screenshot](https://github.com/aspetkov/cad2shape/blob/master/boostlinker.PNG)


## Portable application ready for run

Copy binx64 directory on hard disk.

Run cmd promt and go to binx64. 

Run cad2shap [path to directory with cad files (batch proccessing) or path to one cad file]

# Samples
d:\binx64 cad2shape d:\b\data //convert all cad files in directory

Screen:
![Screenshot](https://github.com/aspetkov/cad2shape/blob/master/startcad2shape.PNG)

Result Screen:
![Screenshot](https://github.com/aspetkov/cad2shape/blob/master/resultfromcad2shape.PNG)


d:\binx64 cad2shape D:\b\Data\02511.cad

Shapefiles will reside in same directory as cad file.



Bulgarian CAD Format (from Geodesy, Cartography and Cadastre Agency) Document

http://www.cadastre.bg/sites/default/files/format_cad_0404_1.doc
