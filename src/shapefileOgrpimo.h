#ifndef SHAPEFILEOGRPIMO_H
#define SHAPEFILEOGRPIMO_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
#include "data_class.h"
using std::string;
using std::vector;

class ShapefileOgrpimo
{
private:
	string nameShapeFile;
	vector<structFieldShape> arrFieldsShapefile;
	structCadasterLayers cadasterLayers;
	structAttrTables attrTables;
	structNomenclatureTables nomTables;
	structReference reference;

	vector<structFieldShape>  initArrFieldsShapefile();


public:
	ShapefileOgrpimo(string nameShapeFileValue,
		structCadasterLayers& cadasterLayersValue,
		structAttrTables& attrTablesValue,
		structNomenclatureTables& nomTablesValue,
		structReference& referenceValue);

	void createShapefile();
};

#endif