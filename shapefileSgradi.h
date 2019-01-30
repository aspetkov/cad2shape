#ifndef SHAPEFILESGRADI_H
#define SHAPEFILESGRADI_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
#include "data_class.h"
using std::string;
using std::vector;

class ShapefileSgradi
{
private:
	string nameShapeFile;
	vector<structFieldShape> arrFieldsShapefileSGRADI;
	structCadasterLayers cadasterLayers;
	structAttrTables attrTables;
	structNomenclatureTables nomTables;
	structReference reference;

	vector<structFieldShape>  initArrFieldsShapefileSGRADI();


public:
	ShapefileSgradi(string nameShapeFileValue,
		structCadasterLayers& cadasterLayersValue,
		structAttrTables& attrTablesValue,
		structNomenclatureTables& nomTablesValue,
		structReference& referenceValue);

	void createShapefileSGRADI();
};

#endif