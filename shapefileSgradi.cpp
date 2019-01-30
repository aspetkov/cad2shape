#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>  
#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileSgradi.h"
#include "helper.h"
#include "helper_template.cpp"
using std::string;
using std::vector;
using std::ofstream;

vector<structFieldShape> ShapefileSgradi::initArrFieldsShapefileSGRADI()
{
	vector<structFieldShape> arrFieldsShapefileSGRADI =
	{
		{ "IDENT", OGRFieldType(OFTString), 20, "tableAttributesSGRADI" },	//C	20	0	1		Идентификатор на сграда
		{ "VIDS", OGRFieldType(OFTInteger), 2, "tableAttributesSGRADI" },		//S	2	0	2		Вид собственост
		{ "CONST", OGRFieldType(OFTInteger), 2, "tableAttributesSGRADI" }, 	//S	2	0	2		Конструкция на сградата
		{ "VFUNC", OGRFieldType(OFTInteger), 3, "tableAttributesSGRADI" }, 	//S	3	0	2		Функционално предназначение на сградата
		{ "VFUNCOLD", OGRFieldType(OFTInteger), 3, "tableAttributesSGRADI" }, 	//S	3	0	2		Функционално предназначение на сградата –(стара номенклатура)
		{ "GOD", OGRFieldType(OFTInteger), 4, "tableAttributesSGRADI" }, 		//S	4	0			Година на построяване на сградата
		{ "PARTIDA", OGRFieldType(OFTString), 20, "tableAttributesSGRADI" }, 	//C	20	0			Номер на партида от ИР
		{ "NOMER1", OGRFieldType(OFTString), 10, "tableAttributesSGRADI" }, 	//C	10	0			Стар планоснимачен номер
		{ "ЕТ", OGRFieldType(OFTInteger), 3, "tableAttributesSGRADI" }, 		//S	3	0			Брой етажи на сградата
		{ "ЕТ1", OGRFieldType(OFTInteger), 3, "tableAttributesSGRADI" }, 		//S	3	0			Брой допълнителни етажи
		{ "BRPOM", OGRFieldType(OFTInteger), 3, "tableAttributesSGRADI" }, 	//S	3	0			Брой самостоятелни обекти
		{ "ADDRCODE", OGRFieldType(OFTInteger), 10, "tableAttributesSGRADI" }, 	//L	10	0	3	ADDRESS	Адрес – код
		{ "LEGAL", OGRFieldType(OFTInteger), 1, "tableAttributesSGRADI" }, 	//S	1	0			Флаг за законност : 0 - няма данни,	1 - незаконна, 2 – законна
		{ "LEGALDOC", OGRFieldType(OFTString), 20, "tableAttributesSGRADI" },  //C	20	0			Описание на документите за законност
		{ "BEG_DATE", OGRFieldType(OFTString), 10, "tableAttributesSGRADI" }, 	//D	10	0			Дата на регистрация
		{ "END_DATE", OGRFieldType(OFTString), 10, "tableAttributesSGRADI" }, 	//D	10	0			Дата на отрегистрация};
//
		{ "VIDS_txt", OGRFieldType(OFTString), 30, "tableNomenclaturеsVIDS" }, // key VIDS;
		{ "VFUNC_txt", OGRFieldType(OFTString), 50, "tableNomenclaturеsVFUNC" }, // key VFUNC;
	};

	return arrFieldsShapefileSGRADI;
};

ShapefileSgradi::ShapefileSgradi(string nameShapeFileValue,
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
	arrFieldsShapefileSGRADI = initArrFieldsShapefileSGRADI();


}

void ShapefileSgradi::createShapefileSGRADI()  
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

	double x, y; 

	const char *pszDriverName = "ESRI Shapefile";
	GDALDriver *poDriver;

	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}

	string nameShapeFileSgradi = nameShapeFile + "_sgradi.shp";

	GDALDataset *poDSSgradi;
	poDSSgradi = poDriver->Create(nameShapeFileSgradi.c_str(), 0, 0, 0, GDT_Unknown, NULL);
	if (poDSSgradi == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayerSgradi;
	poLayerSgradi = poDSSgradi->CreateLayer("sgradi_out", NULL, wkbPolygon, NULL);
	if (poLayerSgradi == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}

	map<std::string, structFieldShape>::iterator itFieldsSgradi;

	for (int i = 0; i < arrFieldsShapefileSGRADI.size(); i++)
	{
		structFieldShape f;
		f = arrFieldsShapefileSGRADI[i];
		const char * ch = f.name.c_str();
		OGRFieldDefn oFieldSgradi(ch, f.ogrType);
		oFieldSgradi.SetWidth(f.lentgh);

		if (poLayerSgradi->CreateField(&oFieldSgradi) != OGRERR_NONE)
		{
			printf("Creating Name field failed.\n");
			exit(1);
		}
	}

	map<std::string, std::string>::iterator itSgradi;

	for (itSgradi = cadasterLayers.cadasterSgradi.begin(); itSgradi != cadasterLayers.cadasterSgradi.end(); itSgradi++)
	{
		cout << itSgradi->first << std::endl; // string (key)
		string sgradaIDENT = itSgradi->first;
		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayerSgradi->GetLayerDefn());

		structDataSGRADI datarowSGRADI;
		datarowSGRADI = attrTables.tableAttributesSGRADI[sgradaIDENT];

		for (int i = 0; i < arrFieldsShapefileSGRADI.size(); i++)
		{
			structFieldShape f;
			f = arrFieldsShapefileSGRADI[i];
			const char * n = f.name.c_str();

			if (f.name == "IDENT")
			{
				const char * ch = datarowSGRADI.IDENT.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VIDS")
			{
				int i = datarowSGRADI.VIDS;
				poFeature->SetField(n, i);
			}
			else if (f.name == "CONST")
			{
				int i = datarowSGRADI.CONST;
				poFeature->SetField(n, i);
			}
			else if (f.name == "VFUNC")
			{
				int i = datarowSGRADI.VFUNC;
				poFeature->SetField(n, i);
			}
			else if (f.name == "VFUNCOLD")
			{
				int i = datarowSGRADI.VFUNCOLD;
				poFeature->SetField(n, i);
			}
			else if (f.name == "GOD")
			{
				int i = datarowSGRADI.GOD;
				poFeature->SetField(n, i);
			}
			else if (f.name == "PARTIDA")
			{
				const char * ch = datarowSGRADI.PARTIDA.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "NOMER1")
			{
				const char * ch = datarowSGRADI.NOMER1.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "ЕТ")
			{
				int i = datarowSGRADI.ЕТ;
				poFeature->SetField(n, i);
			}
			else if (f.name == "ЕТ1")
			{
				int i = datarowSGRADI.ЕТ1;
				poFeature->SetField(n, i);
			}
			else if (f.name == "BRPOM")
			{
				int i = datarowSGRADI.BRPOM;
				poFeature->SetField(n, i);
			}
			else if (f.name == "ADDRCODE")
			{
				int i = datarowSGRADI.ADDRCODE;
				poFeature->SetField(n, i);
			}
			else if (f.name == "LEGAL")
			{
				int i = datarowSGRADI.LEGAL;
				poFeature->SetField(n, i);
			}
			else if (f.name == "LEGALDOC")
			{
				const char * ch = datarowSGRADI.LEGALDOC.c_str();
				poFeature->SetField(n, ch);
			}

			else if (f.name == "BEG_DATE")
			{
				const char * ch = datarowSGRADI.BEG_DATE.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "END_DATE")
			{
				const char * ch = datarowSGRADI.END_DATE.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VIDS_txt")
			{
				structNomTABLEVIDS t;
				t = nomTables.tableNomenclaturеsVIDS[datarowSGRADI.VIDS];
				const char * ch = t.VIDS_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VFUNC_txt")
			{
				structNomTABLEVFUNC t;
				t = nomTables.tableNomenclaturеsVFUNC[datarowSGRADI.VFUNC];
				const char * ch = t.VFUNC_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else
			{
			}
		}

		splitstringsemicolon = split(itSgradi->second, delimiterCharSemicolon);

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

		if (poLayerSgradi->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDSSgradi);

	if (reference.coordtype == "2005")
	{
		std::ofstream myfile;
		myfile.open(nameShapeFile + "_sgradi.prj");
		myfile << R"(PROJCS["BGS_2005",GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.000,298.25722293]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]],PROJECTION["Lambert_Conformal_Conic"],PARAMETER["False_Easting",500000.000],PARAMETER["False_Northing",4725824.359],PARAMETER["Central_Meridian",25.50000000000000],PARAMETER["Latitude_Of_Origin",42.66787563333330],PARAMETER["Standard_Parallel_1",43.33333333000000],PARAMETER["Standard_Parallel_2",42.00000000000000],UNIT["Meter",1.00000000000000]])";
		myfile.close();
	}

}

