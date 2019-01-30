#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>  
#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefilePrava.h"
#include "helper.h"
#include "helper_template.cpp"
using std::string;
using std::vector;
using std::ofstream;


vector<structFieldShape> ShapefilePrava::initArrFieldsShapefilePRAVA()
{
	vector<structFieldShape> arrFieldsShapefilePRAVA =
	{
		{ "IDENT", OGRFieldType(OFTString), 20, "tableAttributesPRAVA" },	//C	20	0	1		Идентификатор на поземлен имот, сграда или самостоятелен обект в сграда
		{ "VIDS", OGRFieldType(OFTInteger), 2, "tableAttributesPRAVA" },		//S	2	0	2		Вид собственост
		{ "PERSON", OGRFieldType(OFTString), 13, "tableAttributesPRAVA" }, //C	13	0	3	PERSONS	ЕГН / Булстат на носителя на правото
		{ "DOCCOD", OGRFieldType(OFTInteger), 10, "tableAttributesPRAVA" }, //L	10	0	3	DOCS	Код на документ
		{ "DOCID1", OGRFieldType(OFTInteger), 3, "tableAttributesPRAVA" }, //S	3	0			Идеална част : стойност на числителя, -1 при проценти, -2 при площ в кв.м.
		{ "DOCID2", OGRFieldType(OFTReal), 8, "tableAttributesPRAVA" }, //N	8	3			Идеална част : знаменател, число на процента или площта в кв.м.площ(в зависимост от DOCID1)
		{ "PLDOC", OGRFieldType(OFTReal), 11, "tableAttributesPRAVA" }, 	//N	11	3			Площ по документ
		{ "PTYPE", OGRFieldType(OFTInteger), 2, "tableAttributesPRAVA" }, //S	2	0	2		Начин на придобиване
		{ "PRAVOVID", OGRFieldType(OFTInteger), 2, "tableAttributesPRAVA" }, //S	2	0	2		Код на вида право
		{ "SROK", OGRFieldType(OFTString), 10, "tableAttributesPRAVA" }, //D	10	0			Крайна дата на правото когато правото е срочно
		{ "DOCIDENT", OGRFieldType(OFTString), 30, "tableAttributesPRAVA" }, //C	30	0			Номер на обекта по документ - свободен текст, например квартал и парцел, пл.сним.номер и т.н.
		{ "DOP", OGRFieldType(OFTInteger), 1, "tableAttributesPRAVA" }, 	//B	1	0			Флаг за допълнителнителен документ(собственост); Т – допълнително, F – основен документ
		{ "BEG_DATE", OGRFieldType(OFTString), 10, "tableAttributesPRAVA" }, //D	10	0			Дата на регистрация на правото
		{ "END_DATE", OGRFieldType(OFTString), 10, "tableAttributesPRAVA" }, //D	10	0			Дата на прекратяване на правото
		{ "END_TIME", OGRFieldType(OFTString), 5, "tableAttributesPRAVA" }, //T	5	0			Време(момент) на прекратяване на правото
		{ "VIDS_txt", OGRFieldType(OFTString), 30, "tableNomenclaturеsVIDS" }, // key VIDS;
		//TABLE PERSONS key PERSON
		{ "SUBTYPE_", OGRFieldType(OFTInteger), 1, "tableAttributesPERSONS" }, // S	1	0	2		Вид на субекта, код от номенклатура
		{ "NAME_", OGRFieldType(OFTString), 45, "tableAttributesPERSONS" },	// C	45	0			Име на собственик
		{ "NSTATE_", OGRFieldType(OFTString), 2, "tableAttributesPERSONS" }, // C	2	0	2		Адрес на собственика - буквен код на страна
		{ "ADDRCODE_", OGRFieldType(OFTInteger), 10, "tableAttributesPERSONS" }, 	// L	10	0	3	ADDRESS	Адрес на собственика - код на адрес
		{ "ADDR_", OGRFieldType(OFTString), 50, "tableAttributesPERSONS" }, 	// C	50	0			Пълен адрес на собственика - коментар
		{ "ADDRET_", OGRFieldType(OFTString), 4, "tableAttributesPERSONS" }, // C	4	0			Адрес на собственика – етаж
		{ "ADDRAP_", OGRFieldType(OFTString), 4, "tableAttributesPERSONS" }, 	// C	4	0			Адрес на собственика – апартамент
		{ "FLAG_", OGRFieldType(OFTInteger), 1, "tableAttributesPERSONS" },     // B	1	0			Флаг за починал собственик
		{ "SPERSON_", OGRFieldType(OFTString), 10, "tableAttributesPERSONS" }, 	// C	10	0			ЕГН на свързано лице(съпруг / съпруга или физическо лице за ЕТ)
		{ "FIRMREG_", OGRFieldType(OFTString), 50, "tableAttributesPERSONS" }, 	// C	50	0			Данни за съдебна регистрация на фирма
		{ "BEG_DATE_", OGRFieldType(OFTString), 10, "tableAttributesPERSONS" }, // D	10	0			Дата на регистрация
		{ "END_DATE_", OGRFieldType(OFTString), 10, "tableAttributesPERSONS" },  // D	10	0			Дата на отрегистрация

	};

	return arrFieldsShapefilePRAVA;
};

ShapefilePrava::ShapefilePrava(string nameShapeFileValue,
	structCadasterLayers& cadasterLayersValue,
	structLayerShemi& layerShemiValue,
	structAttrTables& attrTablesValue,
	structNomenclatureTables& nomTablesValue,
	structReference& referenceValue)
{
	nameShapeFile = nameShapeFileValue;
	cadasterLayers = cadasterLayersValue;
	layerShemi = layerShemiValue;
	attrTables = attrTablesValue;
	nomTables = nomTablesValue;
	reference = referenceValue;
	arrFieldsShapefilePRAVA = initArrFieldsShapefilePRAVA();


}

void ShapefilePrava::createShapefilePRAVA (string layerName)  //(string nameShapeFile, vector<structFieldShape> arrFieldsShapefileIMOTI, structCadasterLayers cadasterLayers, structAttrTables attrTables, structNomenclatureTables nomTables)
{
	string myInputstring;
	string tableFields;
	string nameShapeFilePrava;
	vector <string> splitstringspace;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringcomma;
	vector <string> onePoint;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	char delimiterCharComma = ',';
	char trimCharQUOTE = '\"';

	map<std::string, std::string> layer;
	double x, y; // , x1, y1;

	if (layerName == "IMOTI")
	{
		layer = cadasterLayers.cadasterImoti;
		nameShapeFilePrava = nameShapeFile + "_pravaimoti.shp";
	}
	else if (layerName == "SGRADI")
	{
		layer = cadasterLayers.cadasterSgradi;
		nameShapeFilePrava = nameShapeFile + "_pravasgradi.shp";
	}
	else if (layerName == "SHEMI")
	{
		layer = layerShemi.shemiAparts;
		nameShapeFilePrava = nameShapeFile + "_pravashemi.shp";
	}

	const char *pszDriverName = "ESRI Shapefile";
	GDALDriver *poDriver;

	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}

	GDALDataset *poDSPrava;
	poDSPrava = poDriver->Create(nameShapeFilePrava.c_str(), 0, 0, 0, GDT_Unknown, NULL);
	if (poDSPrava == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayerPrava;
	poLayerPrava = poDSPrava->CreateLayer("prava_out", NULL, wkbPolygon, NULL);
	if (poLayerPrava == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}

	map<std::string, structFieldShape>::iterator itFieldsPrava;

	for (int i = 0; i < arrFieldsShapefilePRAVA.size(); i++)
	{
		structFieldShape f;
		f = arrFieldsShapefilePRAVA[i];
		const char * ch = f.name.c_str();
		OGRFieldDefn oField(ch, f.ogrType);
		oField.SetWidth(f.lentgh);

		if (poLayerPrava->CreateField(&oField) != OGRERR_NONE)
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
		multimap<string, structDataPRAVA>::iterator itAttrTable;
		pair <multimap<string, structDataPRAVA>::iterator, multimap<string, structDataPRAVA>::iterator> range;
		range = attrTables.tableAttributesPRAVA.equal_range(shapeIDENT);


		if (attrTables.tableAttributesPRAVA.count(shapeIDENT) > 0)
		{
			for (itAttrTable = range.first; itAttrTable != range.second; ++itAttrTable)
			{
				OGRFeature *poFeature;
				poFeature = OGRFeature::CreateFeature(poLayerPrava->GetLayerDefn());
				structDataPRAVA datarowPRAVA;
				datarowPRAVA = itAttrTable->second;

				for (int i = 0; i < arrFieldsShapefilePRAVA.size(); i++)

				{
					structFieldShape f;
					f = arrFieldsShapefilePRAVA[i];
					const char * n = f.name.c_str();

					if (f.name == "IDENT")
					{
						const char * ch = shapeIDENT.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "VIDS")
					{
						int i = datarowPRAVA.VIDS;
						poFeature->SetField(n, i);
					}
					else if (f.name == "PERSON")
					{
						const char * ch = datarowPRAVA.PERSON.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "DOCCOD")
					{
						int i = datarowPRAVA.DOCCOD;
						poFeature->SetField(n, i);
					}
					else if (f.name == "DOCID1")
					{
						int i = datarowPRAVA.DOCID1;
						poFeature->SetField(n, i);
					}
					else if (f.name == "DOCID2")
					{
						double d = datarowPRAVA.DOCID2;
						poFeature->SetField(n, d);
					}
					else if (f.name == "PLDOC")
					{
						double d = datarowPRAVA.PLDOC;
						poFeature->SetField(n, d);
					}
					else if (f.name == "PTYPE")
					{
						int i = datarowPRAVA.PTYPE;
						poFeature->SetField(n, i);
					}
					else if (f.name == "PRAVOVID")
					{
						int i = datarowPRAVA.PRAVOVID;
						poFeature->SetField(n, i);
					}
					else if (f.name == "SROK")
					{
						const char * ch = datarowPRAVA.SROK.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "DOCIDENT")
					{
						const char * ch = datarowPRAVA.DOCIDENT.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "DOP")
					{
						int i = datarowPRAVA.DOP;
						poFeature->SetField(n, i);
					}
					else if (f.name == "BEG_DATE")
					{
						const char * ch = datarowPRAVA.BEG_DATE.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "END_DATE")
					{
						const char * ch = datarowPRAVA.END_DATE.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "END_TIME")
					{
						const char * ch = datarowPRAVA.END_TIME.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "VIDS_txt")
					{
						structNomTABLEVIDS t;
						t = nomTables.tableNomenclaturеsVIDS[datarowPRAVA.VIDS];
						const char * ch = t.VIDS_txt.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "SUBTYPE_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						int i = t.SUBTYPE;
						poFeature->SetField(n, i);
					}
					else if (f.name == "NAME_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.NAME.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "NSTATE_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.NSTATE.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "ADDRCODE_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						int i = t.ADDRCODE;
						poFeature->SetField(n, i);
					}
					else if (f.name == "ADDR_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.ADDR.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "ADDRET_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.ADDRET.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "ADDRAP_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.ADDRAP.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "FLAG_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						int i = t.FLAG;
						poFeature->SetField(n, i);
					}
					else if (f.name == "SPERSON_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.SPERSON.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "FIRMREG_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.FIRMREG.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "BEG_DATE_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.BEG_DATE.c_str();
						poFeature->SetField(n, ch);
					}
					else if (f.name == "END_DATE_")
					{
						structDataPERSONS t;
						t = attrTables.tableAttributesPERSONS[datarowPRAVA.PERSON];
						const char * ch = t.END_DATE.c_str();
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

				if (poLayerPrava->CreateFeature(poFeature) != OGRERR_NONE)
				{
					printf("Failed to create feature in shapefile.\n");
					exit(1);
				}
				OGRFeature::DestroyFeature(poFeature);
			}
		}
	}
	GDALClose(poDSPrava);

	if (reference.coordtype == "2005")
	{
		std::ofstream myfile;
		if (layerName == "IMOTI")
		{
			myfile.open(nameShapeFile + "_pravaimoti.prj");
		}
		else if (layerName == "SGRADI")
		{
			myfile.open(nameShapeFile + "_pravasgradi.prj");
		}
		else if (layerName == "SHEMI")
		{
			myfile.open(nameShapeFile + "_pravashemi.prj");
		}
		myfile << R"(PROJCS["BGS_2005",GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.000,298.25722293]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]],PROJECTION["Lambert_Conformal_Conic"],PARAMETER["False_Easting",500000.000],PARAMETER["False_Northing",4725824.359],PARAMETER["Central_Meridian",25.50000000000000],PARAMETER["Latitude_Of_Origin",42.66787563333330],PARAMETER["Standard_Parallel_1",43.33333333000000],PARAMETER["Standard_Parallel_2",42.00000000000000],UNIT["Meter",1.00000000000000]])";
		myfile.close();
	}
}
