#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>  
#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileOgrpimo.h"
#include "helper.h"
#include "helper_template.cpp"
using std::string;
using std::vector;
using std::ofstream;

vector<structFieldShape> ShapefileOgrpimo::initArrFieldsShapefile()
{
	vector<structFieldShape> arrFieldsShapefile =
	{
		////F name	type	Len	dec	flag	table	Име на полето
		{ "IDENT", OGRFieldType(OFTString), 20, "tableAttributesOGRPIMO" },	//F IDENT    C  20 0 3 POZEMLIMOTI
		{ "TYPE", OGRFieldType(OFTInteger), 3, "tableAttributesOGRPIMO" },     //S   3 0 2
		{ "DOCCOD", OGRFieldType(OFTInteger), 10, "tableAttributesOGRPIMO" },   //L  10 0 3 DOCS
		{ "DOCCOD1", OGRFieldType(OFTInteger), 10, "tableAttributesOGRPIMO" },    //L  10 0 3 DOCS
		{ "STATUS", OGRFieldType(OFTInteger), 1, "tableAttributesOGRPIMO" },    //S   1 0 2
		{ "BEG_DATE", OGRFieldType(OFTString), 10, "tableAttributesOGRPIMO" },	//D	10	0			Дата на регистрация на правото
		{ "END_DATE", OGRFieldType(OFTString), 10, "tableAttributesOGRPIMO" },	//D	10	0			Дата на прекратяване на правото

		{ "TYPE_txt", OGRFieldType(OFTString), 250, "tableNomenclaturеsOGRPIMO_TYPE" }, // key TYPE;

	};

	return arrFieldsShapefile;
};

ShapefileOgrpimo::ShapefileOgrpimo(string nameShapeFileValue,
	structCadasterLayers& cadasterLayersValue,
	structAttrTables& attrTablesValue,
	structNomenclatureTables& nomTablesValue,
	structReference& referenceValue)
{
	nameShapeFile = nameShapeFileValue;
	cadasterLayers = cadasterLayersValue;
	attrTables = attrTablesValue;
	nomTables = nomTablesValue;
	reference = referenceValue;
	arrFieldsShapefile = initArrFieldsShapefile();
}

void ShapefileOgrpimo::createShapefile() 
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

	map<std::string, std::string> layer;
	double x, y; 

	layer = cadasterLayers.cadasterImoti;
	string nameShapeFileOGR = nameShapeFile + "_ogrpimo.shp";

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
	poDS = poDriver->Create(nameShapeFileOGR.c_str(), 0, 0, 0, GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayer;
	poLayer = poDS->CreateLayer("out", NULL, wkbPolygon, NULL);
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

	map<std::string, std::string>::iterator it;

	for (it = layer.begin(); it != layer.end(); it++)
	{
		cout << it->first << std::endl; // string (key)
		string shapeIDENT = it->first;
		multimap<string, structDataOGRPIMO>::iterator itAttrTable;
		pair <multimap<string, structDataOGRPIMO>::iterator, multimap<string, structDataOGRPIMO>::iterator> range;
		range = attrTables.tableAttributesOGRPIMO.equal_range(shapeIDENT);


		if (attrTables.tableAttributesOGRPIMO.count(shapeIDENT) > 0)
		{
			for (itAttrTable = range.first; itAttrTable != range.second; ++itAttrTable)
			{
				OGRFeature *poFeature;
				poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
				structDataOGRPIMO datarow;
				datarow = itAttrTable->second;


				for (int i = 0; i < arrFieldsShapefile.size(); i++)

				{
					structFieldShape f;
					f = arrFieldsShapefile[i];
					const char * n = f.name.c_str();

					if (f.name == "IDENT")
					{
						const char * ch = shapeIDENT.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "TYPE")
					{
						int i = datarow.TYPE;
						poFeature->SetField(n, i);
					}
					else if (f.name == "DOCCOD")
					{
						int i = datarow.DOCCOD;
						poFeature->SetField(n, i);
					}
					else if (f.name == "DOCCOD1")
					{
						int i = datarow.DOCCOD1;
						poFeature->SetField(n, i);
					}
					else if (f.name == "STATUS")
					{
						int i = datarow.STATUS;
						poFeature->SetField(n, i);
					}
					else if (f.name == "BEG_DATE")
					{
						const char * ch = datarow.BEG_DATE.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "END_DATE")
					{
						const char * ch = datarow.END_DATE.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "TYPE_txt")
					{
						structNomTABLEOGRPIMO_TYPE t;
						t = nomTables.tableNomenclaturеsOGRPIMO_TYPE[datarow.TYPE];
						const char * ch = t.TYPE_txt.c_str();
						poFeature->SetField(n, ch);
					}
					else
					{
					}
				}
				splitstringsemicolon = split(it->second, delimiterCharSemicolon);

				OGRLinearRing ring;
				OGRPolygon poly;
				for (int i = 0; i < splitstringsemicolon.size(); i++)
				{
					splitstringsemicolon[i] = trim(splitstringsemicolon[i]);
					onePoint = split(splitstringsemicolon[i], delimiterCharSpace);
					y = atod(onePoint[1]) + reference.X;
					x = atod(onePoint[2]) + reference.Y;
					ring.addPoint(x, y);
				}

				ring.closeRings();
				poly.addRing(&ring);
				poFeature->SetGeometry(&poly);

				if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
				{
					printf("Failed to create feature in shapefile.\n");
					exit(1);
				}
				OGRFeature::DestroyFeature(poFeature);
			}
		}
	}
	GDALClose(poDS);
	if (reference.coordtype == "2005")
	{
		std::ofstream myfile;
		myfile.open(nameShapeFile + "_ogrpimo.prj");
		myfile << R"(PROJCS["BGS_2005",GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.000,298.25722293]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]],PROJECTION["Lambert_Conformal_Conic"],PARAMETER["False_Easting",500000.000],PARAMETER["False_Northing",4725824.359],PARAMETER["Central_Meridian",25.50000000000000],PARAMETER["Latitude_Of_Origin",42.66787563333330],PARAMETER["Standard_Parallel_1",43.33333333000000],PARAMETER["Standard_Parallel_2",42.00000000000000],UNIT["Meter",1.00000000000000]])";
		myfile.close();
	}

}

