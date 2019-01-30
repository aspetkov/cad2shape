#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>  
#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileShemi.h"
#include "helper.h"
#include "helper_template.cpp"
using std::string;
using std::vector;
using std::ofstream;

vector<structFieldShape> ShapefileShemi::initArrFieldsShapefileSHEMI()
{
	vector<structFieldShape> arrFieldsShapefileSHEMI =
	{
		{ "IDENT", OGRFieldType(OFTString), 20, "tableAttributesAPARTS" }, //C	20	0	1		Идентификатор на самостоятелен обект
		{ "REM", OGRFieldType(OFTString), 20, "tableAttributesAPARTS" },  //	C	20	0			Описание на обекта
		{ "PREDN", OGRFieldType(OFTInteger), 3, "tableAttributesAPARTS" }, //	S	3	0	2		Предназначение на самостоятелен обект
		{ "VIDS", OGRFieldType(OFTInteger), 2, "tableAttributesAPARTS" }, //S	2	0	2		Вид собственост
		{ "PARTIDA", OGRFieldType(OFTString), 20, "tableAttributesAPARTS" }, //	C	20	0			Номер на партида от ИР
		{ "ADDRCODE", OGRFieldType(OFTInteger), 10, "tableAttributesAPARTS" }, //	L	10	0	3	ADDRESS	Адрес – код
		{ "ADDRET", OGRFieldType(OFTString), 4, "tableAttributesAPARTS" },  //C	4	0			Адрес – етаж
		{ "ADDRAP", OGRFieldType(OFTString), 4, "tableAttributesAPARTS" },  //C	4	0			Адрес – апартамент
		{ "PLDOC", OGRFieldType(OFTReal), 11, "tableAttributesAPARTS" }, 	//N	11	3			Площ по документ
		{ "BRET", OGRFieldType(OFTInteger), 2, "tableAttributesAPARTS" }, //F BRET	S	2	0			Брой етажи на обекта
		{ "DOPS", OGRFieldType(OFTString), 80, "tableAttributesAPARTS" }, //F DOPS	C	80	0			Вид и площ на прилежащите помещения и общите части, които са неразделна част от обекта(свободен текст)
		{"BEG_DATE", OGRFieldType(OFTString), 10, "tableAttributesAPARTS" }, //structField BEG_DATE;	//D	10	0			Дата на регистрация
		{ "END_DATE", OGRFieldType(OFTString), 10, "tableAttributesAPARTS" }, //structField END_DATE;	//D	10	0			Дата на отрегистрация};
		{ "VIDS_txt", OGRFieldType(OFTString), 30, "tableNomenclaturеsVIDS" }, // key VIDS;
		{ "PREDN_txt", OGRFieldType(OFTString), 50, "tableNomenclaturеsPREDN" }, // key PREDN;
	};
	 
	return arrFieldsShapefileSHEMI;
};

ShapefileShemi::ShapefileShemi(string nameShapeFileValue,
	structLayerShemi& layerShemiValue,
	structAttrTables& attrTablesValue,
	structNomenclatureTables& nomTablesValue,
	structReference& referenceValue)
{
	nameShapeFile = nameShapeFileValue;
	layerShemi = layerShemiValue;
	attrTables = attrTablesValue;
	nomTables = nomTablesValue;
	reference = referenceValue;
	arrFieldsShapefileSHEMI = initArrFieldsShapefileSHEMI();


}

void ShapefileShemi::createShapefileSHEMI()  //(string nameShapeFile, vector<structFieldShape> arrFieldsShapefileIMOTI, structCadasterLayers cadasterLayers, structAttrTables attrTables, structNomenclatureTables nomTables)
{
	string myInputstring;
	string tableFields;
	vector <string> splitstringspace;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringcomma;
	vector <string> onePoint;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	char delimiterCharComma = ',';
	char trimCharQUOTE = '\"';

	double x, y; // , x1, y1;

	const char *pszDriverName = "ESRI Shapefile";
	GDALDriver *poDriver;

	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}
	
			//SHEMI
			string nameShapeFileShemi = nameShapeFile + "_shemi.shp";
			GDALDataset *poDSShemi;
			poDSShemi = poDriver->Create(nameShapeFileShemi.c_str(), 0, 0, 0, GDT_Unknown, NULL);
			if (poDSShemi == NULL)
			{
				printf("Creation of output file failed.\n");
				exit(1);
			}
	
			OGRLayer *poLayerShemi;
			poLayerShemi = poDSShemi->CreateLayer("shemi_out", NULL, wkbPolygon, NULL);
			if (poLayerShemi == NULL)
			{
				printf("Layer creation failed.\n");
				exit(1);
			}
	

	map<std::string, structFieldShape>::iterator itFieldsShemi;

	for (int i = 0; i < arrFieldsShapefileSHEMI.size(); i++)
	{
		structFieldShape f;
		f = arrFieldsShapefileSHEMI[i];
		const char * ch = f.name.c_str();
		OGRFieldDefn oFieldShemi(ch, f.ogrType);
		oFieldShemi.SetWidth(f.lentgh);

		if (poLayerShemi->CreateField(&oFieldShemi) != OGRERR_NONE)
		{
			printf("Creating Name field failed.\n");
			exit(1);
		}
	}

		map<std::string, std::string>::iterator itShemi;
	
	for (itShemi = layerShemi.shemiAparts.begin(); itShemi != layerShemi.shemiAparts.end(); itShemi++)
	{
		std::cout << itShemi->first << std::endl; // string (key)
				
		string shemiIDENT = itShemi->first;


		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayerShemi->GetLayerDefn());

		structDataAPARTS datarowAPARTS;
		datarowAPARTS = attrTables.tableAttributesAPARTS[shemiIDENT];

		for (int i = 0; i < arrFieldsShapefileSHEMI.size(); i++)
		{
			structFieldShape f;
			f = arrFieldsShapefileSHEMI[i];
			const char * n = f.name.c_str();

			if (f.name == "IDENT")
			{
				const char * ch = datarowAPARTS.IDENT.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "REM")
			{
				const char * ch = datarowAPARTS.REM.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "PREDN")
			{
				int i = datarowAPARTS.PREDN;
				poFeature->SetField(n, i);
			}
			else if (f.name == "VIDS")
			{
				int i = datarowAPARTS.VIDS;
				poFeature->SetField(n, i);
			}
			else if (f.name == "PARTIDA")
			{
				const char * ch = datarowAPARTS.PARTIDA.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "ADDRCODE")
			{
				int i = datarowAPARTS.ADDRCODE;
				poFeature->SetField(n, i);
			}
			else if (f.name == "ADDRET")
			{
				const char * ch = datarowAPARTS.ADDRET.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "ADDRAP")
			{
				const char * ch = datarowAPARTS.ADDRAP.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "PLDOC")
			{
				double d = datarowAPARTS.PLDOC;
				poFeature->SetField(n, d);
			}
			else if (f.name == "BRET")
			{
				int i = datarowAPARTS.BRET;
				poFeature->SetField(n, i);
			}
			else if (f.name == "DOPS")
			{
				const char * ch = datarowAPARTS.ADDRAP.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "BEG_DATE")
			{
				const char * ch = datarowAPARTS.BEG_DATE.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "END_DATE")
			{
				const char * ch = datarowAPARTS.END_DATE.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VIDS_txt")
			{
				structNomTABLEVIDS t;
				t = nomTables.tableNomenclaturеsVIDS[datarowAPARTS.VIDS];
				const char * ch = t.VIDS_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "PREDN_txt")
			{
				structNomTABLEPREDN t;
				t = nomTables.tableNomenclaturеsPREDN[datarowAPARTS.PREDN];
				const char * ch = t.PREDN_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else
			{
			}
		}

		splitstringsemicolon = split(itShemi->second, delimiterCharSemicolon);

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

		if (poLayerShemi->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDSShemi);
	if (reference.coordtype == "2005")
	{
		std::ofstream myfile;
		myfile.open(nameShapeFile + "_shemi.prj");
		myfile << R"(PROJCS["BGS_2005",GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.000,298.25722293]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]],PROJECTION["Lambert_Conformal_Conic"],PARAMETER["False_Easting",500000.000],PARAMETER["False_Northing",4725824.359],PARAMETER["Central_Meridian",25.50000000000000],PARAMETER["Latitude_Of_Origin",42.66787563333330],PARAMETER["Standard_Parallel_1",43.33333333000000],PARAMETER["Standard_Parallel_2",42.00000000000000],UNIT["Meter",1.00000000000000]])";
		myfile.close();
	}

}

