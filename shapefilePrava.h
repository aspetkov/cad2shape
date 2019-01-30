#ifndef SHAPEFILEPRAVA_H
#define SHAPEFILEPRAVA_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
#include "data_class.h"
using std::string;
using std::vector;

class ShapefilePrava
{
private:
	string nameShapeFile;
	vector<structFieldShape> arrFieldsShapefilePRAVA;
	structCadasterLayers cadasterLayers;
	structLayerShemi layerShemi;
	structAttrTables attrTables;
	structNomenclatureTables nomTables;
	structReference reference;

	vector<structFieldShape>  initArrFieldsShapefilePRAVA();

public:
	ShapefilePrava(string nameShapeFileValue,
		structCadasterLayers& cadasterLayersValue,
		structLayerShemi& layerShemiValue,
		structAttrTables& attrTablesValue,
		structNomenclatureTables& nomTablesValue,
		structReference& referenceValue);

	void createShapefilePRAVA(string layerName);
};

#endif