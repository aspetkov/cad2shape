#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>  
#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileNadpisi.h"
#include "helper.h"
#include "helper_template.cpp"
using std::string;
using std::vector;
using std::ofstream;

vector<structFieldShape> ShapefileNadpisi::initArrFieldsShapefile()
{
	vector<structFieldShape> arrFieldsShapefile =
	{
		{ "type", OGRFieldType(OFTInteger), 2, "nadpisiAttr" }, // тип на текста по класификатора в приложение № 1;   featureBufferT.set_Value(T_TypeKlasificator, System.Convert.ToInt16(splitstringspace[1]));
		{ "code", OGRFieldType(OFTString), 10, "nadpisiAttr" },  //  е уникален номер на текста;   featureBufferT.set_Value(T_CodeText, System.Convert.ToInt32(splitstringspace[2]));
		{ "pointX", OGRFieldType(OFTReal), 8, "nadpisiAttr" }, // са координати на текста;   double pointTextX = System.Convert.ToDouble(splitstringspace[4]) + referenceY;
		{ "pointY", OGRFieldType(OFTReal), 8, "nadpisiAttr" },  // са координати на текста;   double pointTextY = System.Convert.ToDouble(splitstringspace[3]) + referenceX;
		{ "sizeFont", OGRFieldType(OFTInteger), 3, "nadpisiAttr" },  //  височина на надписа в милиметри на хартията  //featureBufferT.set_Value(T_SizeFont, System.Convert.ToInt32(splitstringspace[5]));
		{ "begDate", OGRFieldType(OFTString), 10, "nadpisiAttr" },  //  е дата на легалната поява на обекта във форма  //featureBufferT.set_Value(T_CreateDate, splitstringspace[6]);
		{ "endDate", OGRFieldType(OFTString), 10, "nadpisiAttr" },  //  е дата на преустановяване на легалното съществуване обекта  //featureBufferT.set_Value(T_EndDate, splitstringspace[7]);
		{ "rotAngle", OGRFieldType(OFTReal), 8, "nadpisiAttr" }, //  е ъгъл на завъртане на текста(100 гона за хоризонтален)  //featureBufferT.set_Value(T_RotAngle, System.Convert.ToDouble(splitstringspace[8]));
		{ "alignment", OGRFieldType(OFTString), 3, "nadpisiAttr" }, // двубуквен код за подравняване   //featureBufferT.set_Value(T_Alignment, splitstringspace[9]);
		{ "textNadpis", OGRFieldType(OFTString), 50, "nadpisiAttr" }, 
		//
		{ "TYPE_txt", OGRFieldType(OFTString), 50, "tableNomenclaturesNADPISI" }, // key type;
	};

	return arrFieldsShapefile;
};

ShapefileNadpisi::ShapefileNadpisi(string nameShapeFileValue,
	structCadasterLayers& cadasterLayersValue,
	structNomenclatureTables& nomTablesValue,
	structReference& referenceValue)
{
	nameShapeFile = nameShapeFileValue;
	cadasterLayers = cadasterLayersValue;
	nomTables = nomTablesValue;
	reference = referenceValue;
	arrFieldsShapefile = initArrFieldsShapefile();
}

void ShapefileNadpisi::createShapefile() 
{
	string myInputstring;
	string tableFields;
//	string nameShapeFile;
	vector <string> splitstringspace;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringcomma;
	vector <string> onePoint;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	char delimiterCharComma = ',';
	char trimCharQUOTE = '\"';

	map<std::string, structNadpisiAtrr> layer;

	layer = cadasterLayers.nadpisiAttr;
	string nameShapeFileNadpisi = nameShapeFile + "_nadpisi.shp";

	const char *pszDriverName = "ESRI Shapefile";
	GDALDriver *poDriver;

	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}

	GDALDataset *poDS;
	poDS = poDriver->Create(nameShapeFileNadpisi.c_str(), 0, 0, 0, GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayer;
	poLayer = poDS->CreateLayer("out", NULL, wkbPoint, NULL);
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}

	for (int i = 0; i < arrFieldsShapefile.size(); i++)
	{
		structFieldShape f;
		f = arrFieldsShapefile[i];
		const char * ch = f.name.c_str();
		OGRFieldDefn oField(ch, f.ogrType);
		oField.SetWidth(f.lentgh);

		if (poLayer->CreateField(&oField) != OGRERR_NONE)
		{
			printf("Creating Name field failed.\n");
			exit(1);
		}
	}

	map<std::string, structNadpisiAtrr>::iterator it;

	for (it = layer.begin(); it != layer.end(); it++)
	{
		
		structNadpisiAtrr t = it->second;
		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());

		for (int i = 0; i < arrFieldsShapefile.size(); i++)

		{
			structFieldShape f;
			f = arrFieldsShapefile[i];
			const char * n = f.name.c_str();

			if (f.name == "type")
			{
				int i = t.type;
				poFeature->SetField(n, i);
			}
			else if (f.name == "code")
			{
				const char * ch = t.code.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "pointX")
			{
				double d = t.pointX;
				poFeature->SetField(n, d);
			}
			else if (f.name == "pointY")
			{
				double d = t.pointY;
				poFeature->SetField(n, d);
			}
			else if (f.name == "sizeFont")
			{
				int i = t.sizeFont;
				poFeature->SetField(n, i);
			}
			else if (f.name == "begDate")
			{
				const char * ch = t.begDate.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "endDate")
			{
				const char * ch = t.endDate.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "rotAngle")
			{
				double d = t.rotAngle;
				poFeature->SetField(n, d);
			}
			else if (f.name == "alignment")
			{
				const char * ch = t.alignment.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "textNadpis")
			{
				const char * ch = t.textNadpis.c_str();
				poFeature->SetField(n, ch);
			}

			else if (f.name == "TYPE_txt")
			{
				structNomTABLENADPISI s;
				s = nomTables.tableNomenclaturеsNADPISI[t.type];
				const char * ch = s.TYPE_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else
			{
			}
		}

		OGRPoint pt;
		pt.setX(t.pointX + reference.Y);
		pt.setY(t.pointY + reference.X);
		poFeature->SetGeometry(&pt);
		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDS);
	if (reference.coordtype == "2005")
	{
		std::ofstream myfile;
		myfile.open(nameShapeFile + "_nadpisi.prj");
		myfile << R"(PROJCS["BGS_2005",GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.000,298.25722293]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]],PROJECTION["Lambert_Conformal_Conic"],PARAMETER["False_Easting",500000.000],PARAMETER["False_Northing",4725824.359],PARAMETER["Central_Meridian",25.50000000000000],PARAMETER["Latitude_Of_Origin",42.66787563333330],PARAMETER["Standard_Parallel_1",43.33333333000000],PARAMETER["Standard_Parallel_2",42.00000000000000],UNIT["Meter",1.00000000000000]])";
		myfile.close();
	}

}

