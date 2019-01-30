#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>  
#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileImoti.h"
#include "helper.h"
#include "helper_template.cpp"
using std::string;
using std::vector;
using std::ofstream;

ShapefileImoti::ShapefileImoti(string nameShapeFileValue,
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
	arrFieldsShapefileIMOTI =		//initArrFieldsShapefileIMOTI();
	{
		{ "IDENT", OGRFieldType(OFTString), 20, "tableAttributesPOZEMLIMOTI" }, //C	20	0	1		Идентификатор на имот
		{ "VIDT", OGRFieldType(OFTInteger), 1, "tableAttributesPOZEMLIMOTI" }, // VIDT;		//S	1	0	2		Вид територия
		{ "VIDTOLD", OGRFieldType(OFTInteger), 1, "tableAttributesPOZEMLIMOTI" }, //structField VIDTOLD;		//S	1	0	2		Вид територия(стара номенклатура)
		{ "VIDS", OGRFieldType(OFTInteger), 2, "tableAttributesPOZEMLIMOTI" }, //structField VIDS;		//S	2	0	2		Вид собственост
		{ "NTP", OGRFieldType(OFTInteger), 4, "tableAttributesPOZEMLIMOTI" }, //structField NTP;		//S	4	0	2		Начин на трайно ползване
		{ "NTPOLD", OGRFieldType(OFTInteger), 4, "tableAttributesPOZEMLIMOTI" }, //structField NTPOLD;		//S	4	0	2		Начин на трайно ползване(стара номенклатура)
		{ "MESTNOST", OGRFieldType(OFTInteger), 4, "tableAttributesPOZEMLIMOTI" }, //structField MESTNOST;	//S	4	0	3	MESTNOSTI	Код на местност
		{ "PARTIDA", OGRFieldType(OFTString), 20, "tableAttributesPOZEMLIMOTI" }, //structField PARTIDA;	//C	20	0			Номер на партида от ИР
		{ "ADDRCODE", OGRFieldType(OFTInteger), 10, "tableAttributesPOZEMLIMOTI" }, //structField ADDRCODE;	//L	10	0	3	ADDRESS	Адрес на имота
		{ "NOMER1", OGRFieldType(OFTString), 10, "tableAttributesPOZEMLIMOTI" }, //structField NOMER1;	//C	10	0			Стар планоснимачен номер
		{ "KVARTAL", OGRFieldType(OFTString), 10, "tableAttributesPOZEMLIMOTI" }, //structField KVARTAL;	//C	10	0			Номер на квартал от регулационен план
		{ "PARCEL", OGRFieldType(OFTString), 10, "tableAttributesPOZEMLIMOTI" }, //structField PARCEL; 	//C	10	0			Номер на парцел  от регулационен план с римски цифри
		{ "GODCAD", OGRFieldType(OFTReal), 4, "tableAttributesPOZEMLIMOTI" }, //structField GODCAD;	//N	4	0			Година на стария кадастрален план
		{ "GODREG", OGRFieldType(OFTReal), 4, "tableAttributesPOZEMLIMOTI" }, //structField GODREG;	//N	4	0			Година на стария регулационен план
		{ "CODZAP", OGRFieldType(OFTInteger), 4, "tableAttributesPOZEMLIMOTI" }, //structField CODZAP;		//S	4	0	3	ZAPOVEDI	Код на последната заповед за одобряване
		{ "ZACON", OGRFieldType(OFTInteger), 2, "tableAttributesPOZEMLIMOTI" }, //structField ZACON;		//S	2	0	2		Код на закон, по който са установени границите
		{ "KAT", OGRFieldType(OFTInteger), 2, "tableAttributesPOZEMLIMOTI" }, //structField KAT;			//S	2	0			Преобладаваща категория на земята
		{ "NVAST", OGRFieldType(OFTInteger), 1, "tableAttributesPOZEMLIMOTI" }, //structField NVAST;		//S	1	0	2		Начин на възстановяване - от класификатора за начините на възстановяване
		{ "VAVOD", OGRFieldType(OFTString), 1, "tableAttributesPOZEMLIMOTI" }, //structField VAVOD;		//B	1	0			флаг за въвод във владение
		{ "BEG_DATE", OGRFieldType(OFTString), 10, "tableAttributesPOZEMLIMOTI" }, //structField BEG_DATE;	//D	10	0			Дата на регистрация
		{ "END_DATE", OGRFieldType(OFTString), 10, "tableAttributesPOZEMLIMOTI" }, //structField END_DATE;	//D	10	0			Дата на отрегистрация};
		{ "VIDS_txt", OGRFieldType(OFTString), 30, "tableNomenclaturеsVIDS" }, // key VIDS;
		{ "VIDT_txt", OGRFieldType(OFTString), 30, "tableNomenclaturеsVIDT" }, // key VIDT;
		{ "NTP_txt", OGRFieldType(OFTString), 30, "tableNomenclaturеsNTP" }, // key NTP;
		{ "MESTN_txt", OGRFieldType(OFTString), 30, "tableAttributesMESTNOSTI" } // key MESTNOST;
	};
}

void ShapefileImoti::createShapefileIMOTI()  
{
	vector <string> splitstringsemicolon;
	vector <string> onePoint;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
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
	//IMOTI

	string nameShapeFileImoti = nameShapeFile + "_imoti.shp";
	
	GDALDataset *poDSImoti;
	poDSImoti = poDriver->Create(nameShapeFileImoti.c_str(), 0, 0, 0, GDT_Unknown, NULL);
	if (poDSImoti == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayerImoti;
	poLayerImoti = poDSImoti->CreateLayer("imoti_out", NULL, wkbPolygon, NULL);
	if (poLayerImoti == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}

	for (int i = 0; i < arrFieldsShapefileIMOTI.size(); i++)
	{
		structFieldShape f;
		f = arrFieldsShapefileIMOTI[i];
		const char * ch = f.name.c_str();
		OGRFieldDefn oFieldImoti(ch, f.ogrType);
		oFieldImoti.SetWidth(f.lentgh);

		if (poLayerImoti->CreateField(&oFieldImoti) != OGRERR_NONE)
		{
			printf("Creating Name field failed.\n");
			exit(1);
		}
	}

	map<std::string, std::string>::iterator itImoti;

	for (itImoti = cadasterLayers.cadasterImoti.begin(); itImoti != cadasterLayers.cadasterImoti.end(); itImoti++)
	{
		cout << itImoti->first << std::endl; 
		string imotIDENT = itImoti->first;
		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayerImoti->GetLayerDefn());

		structDataPOZEMLIMOTI datarowPOZEMLIMOTI;
		datarowPOZEMLIMOTI = attrTables.tableAttributesPOZEMLIMOTI[imotIDENT];

		for (int i = 0; i < arrFieldsShapefileIMOTI.size(); i++)
		{
			structFieldShape f;
			f = arrFieldsShapefileIMOTI[i];
			const char * n = f.name.c_str();

			if (f.name == "IDENT")
			{
				const char * ch = imotIDENT.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VIDT")
			{
				int i = datarowPOZEMLIMOTI.VIDT;
				poFeature->SetField(n, i);
			}
			else if (f.name == "VIDTOLD")
			{
				int i = datarowPOZEMLIMOTI.VIDTOLD;
				poFeature->SetField(n, i);
			}
			else if (f.name == "VIDS")
			{
				int i = datarowPOZEMLIMOTI.VIDS;
				poFeature->SetField(n, i);
			}
			else if (f.name == "NTP")
			{
				int i = datarowPOZEMLIMOTI.VIDT;
				poFeature->SetField(n, i);
			}
			else if (f.name == "NTPOLD")
			{
				int i = datarowPOZEMLIMOTI.NTPOLD;
				poFeature->SetField(n, i);
			}
			else if (f.name == "MESTNOST")
			{
				int i = datarowPOZEMLIMOTI.MESTNOST;
				poFeature->SetField(n, i);
			}
			else if (f.name == "PARTIDA")
			{
				const char * ch = datarowPOZEMLIMOTI.PARTIDA.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "ADDRCODE")
			{
				int i = datarowPOZEMLIMOTI.ADDRCODE;
				poFeature->SetField(n, i);
			}
			else if (f.name == "NOMER1")
			{
				const char * ch = datarowPOZEMLIMOTI.IDENT.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "KVARTAL")
			{
				const char * ch = datarowPOZEMLIMOTI.KVARTAL.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "PARCEL")
			{
				const char * ch = datarowPOZEMLIMOTI.PARCEL.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "GODCAD")
			{
				double d = datarowPOZEMLIMOTI.GODCAD;
				poFeature->SetField(n, d);
			}
			else if (f.name == "GODREG")
			{
				double d = datarowPOZEMLIMOTI.GODREG;
				poFeature->SetField(n, d);
			}
			else if (f.name == "CODZAP")
			{
				int i = datarowPOZEMLIMOTI.CODZAP;
				poFeature->SetField(n, i);
			}
			else if (f.name == "ZACON")
			{
				int i = datarowPOZEMLIMOTI.VIDT;
				poFeature->SetField(n, i);
			}
			else if (f.name == "KAT")
			{
				int i = datarowPOZEMLIMOTI.KAT;
				poFeature->SetField(n, i);
			}
			else if (f.name == "NVAST")
			{
				int i = datarowPOZEMLIMOTI.VIDT;
				poFeature->SetField(n, i);
			}
			else if (f.name == "VAVOD")
			{
				int i = datarowPOZEMLIMOTI.VAVOD;
				poFeature->SetField(n, i);
			}
			else if (f.name == "BEG_DATE")
			{
				const char * ch = datarowPOZEMLIMOTI.BEG_DATE.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "END_DATE")
			{
				const char * ch = datarowPOZEMLIMOTI.END_DATE.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VIDS_txt")
			{
				structNomTABLEVIDS t;
				t = nomTables.tableNomenclaturеsVIDS[datarowPOZEMLIMOTI.VIDS];
				const char * ch = t.VIDS_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "VIDT_txt")
			{
				structNomTABLEVIDT t;
				t = nomTables.tableNomenclaturеsVIDT[datarowPOZEMLIMOTI.VIDT];
				const char * ch = t.VIDT_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "NTP_txt")
			{
				structNomTABLENTP t;
				t = nomTables.tableNomenclaturеsNTP[datarowPOZEMLIMOTI.NTP];
				const char * ch = t.NTP_txt.c_str();
				poFeature->SetField(n, ch);
			}
			else if (f.name == "MESTN_txt")
			{
				structDataMESTNOSTI t;
				t = attrTables.tableAttributesMESTNOSTI[datarowPOZEMLIMOTI.MESTNOST];
				const char * ch = t.NAME.c_str();
				poFeature->SetField(n, ch);
			}
			else
			{
			}
		}

		splitstringsemicolon = split(itImoti->second, delimiterCharSemicolon);

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

		if (poLayerImoti->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDSImoti);

	if (reference.coordtype == "2005")
	{
		std::ofstream myfile;
		myfile.open(nameShapeFile + "_imoti.prj");
		myfile << R"(PROJCS["BGS_2005",GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.000,298.25722293]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]],PROJECTION["Lambert_Conformal_Conic"],PARAMETER["False_Easting",500000.000],PARAMETER["False_Northing",4725824.359],PARAMETER["Central_Meridian",25.50000000000000],PARAMETER["Latitude_Of_Origin",42.66787563333330],PARAMETER["Standard_Parallel_1",43.33333333000000],PARAMETER["Standard_Parallel_2",42.00000000000000],UNIT["Meter",1.00000000000000]])";
		myfile.close();
	}
}


