#ifndef SHAPEFILEIMOTI_H
#define SHAPEFILEIMOTI_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
#include "data_class.h"
using std::string;
using std::vector;

class ShapefileImoti
{
private:
	string nameShapeFile;
	vector<structFieldShape> arrFieldsShapefileIMOTI;
	structCadasterLayers cadasterLayers;
	structAttrTables attrTables;
	structNomenclatureTables nomTables;
	structReference reference;

public:
	ShapefileImoti(string nameShapeFileValue, 
				   structCadasterLayers& cadasterLayersValue, 
				   structAttrTables& attrTablesValue, 
				   structNomenclatureTables& nomTablesValue,
				   structReference& referenceValue);

	void createShapefileIMOTI();

};
#endif