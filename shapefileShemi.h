#ifndef SHAPEFILESHEMI_H
#define SHAPEFILESHEMI_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
#include "data_class.h"
using std::string;
using std::vector;

class ShapefileShemi
{
private:
	string nameShapeFile;
	vector<structFieldShape> arrFieldsShapefileSHEMI;
	structLayerShemi layerShemi;
	structAttrTables attrTables;
	structNomenclatureTables nomTables;
	structReference reference;

	vector<structFieldShape>  initArrFieldsShapefileSHEMI();

public:
	ShapefileShemi(string nameShapeFileValue,
		structLayerShemi& layerShemiValue,
		structAttrTables& attrTablesValue,
		structNomenclatureTables& nomTablesValue,
		structReference& referenceValue);

	void createShapefileSHEMI();
};

#endif