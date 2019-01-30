# cad2shape
## Convert bulgarian CAD format (from Geodesy, Cartography and Cadastre Agency) to shapefile 
### Full featured project with Visual Studio 2013, C++,  GDAL OGR and boost/filesystem

How to start development:
Download the project cad2shape.
Download GDAL Development Kits from:
http://www.gisinternals.com/query.html?content=filelist&file=release-1800-x64-dev.zip
Download and compile boost for VS2013 64 bit.

## Portable application ready for run
Copy binx64 directory on hard disk.
Run cmd promt and go to binx64. 
Run cad2shap [path to directory with cad files (batch proccessing) or path to one cad file]
# Samples
d:\binx64 cad2shape d:\b\data //convert all cad files in directory
d:\binx64 cad2shape D:\b\Data\02511.cad

Shapefiles will reside in same directory as cad file.
