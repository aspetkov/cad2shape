#ifndef SHAPEFILENADPISI_H
#define SHAPEFILENADPISI_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
#include "data_class.h"
using std::string;
using std::vector;

class ShapefileNadpisi
{
private:
	string nameShapeFile;
	vector<structFieldShape> arrFieldsShapefile;
	structCadasterLayers cadasterLayers;
	structNomenclatureTables nomTables;
	structReference reference;

	vector<structFieldShape>  initArrFieldsShapefile();


public:
	ShapefileNadpisi(string nameShapeFileValue,
		structCadasterLayers& cadasterLayersValue,
		structNomenclatureTables& nomTablesValue,
		structReference& referenceValue);

	void createShapefile();


};



#endif