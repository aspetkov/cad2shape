#ifndef DATACLASS_H
#define DATACLASS_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
using std::string;
using std::vector;

const int fieldsCountPOZEMLIMOTI = 21;
const int fieldsCountMESTNOSTI = 4;
const int fieldsCountPRAVA = 15;
const int fieldsCountPERSONS = 13;
const int fieldsCountAPARTS = 13;
const int fieldsCountSGRADI = 16;
const int fieldsCountOGRIMO = 7;

struct structReference
{
	double X = 0;
	double Y = 0;
	std::string coordtype = "";
 };

struct structCharArray
{
	vector<char>  pChars;
//	char* pChars;
	int sizeCharArray = 0;
	int currentChar = 0;
};

struct structConturAttr
{
	int typeContur;
	double pointInConturX;
	double pointInConturY;
};

struct structNadpisiAtrr
{
	int type; // тип на текста по класификатора в приложение № 1;   featureBufferT.set_Value(T_TypeKlasificator, System.Convert.ToInt16(splitstringspace[1]));
	string code; //  е уникален номер на текста;   featureBufferT.set_Value(T_CodeText, System.Convert.ToInt32(splitstringspace[2]));
	double pointX; // са координати на текста;   double pointTextX = System.Convert.ToDouble(splitstringspace[4]) + referenceY;
	double pointY; // са координати на текста;   double pointTextY = System.Convert.ToDouble(splitstringspace[3]) + referenceX;
	int sizeFont; //  височина на надписа в милиметри на хартията  //featureBufferT.set_Value(T_SizeFont, System.Convert.ToInt32(splitstringspace[5]));
	string begDate; //  е дата на легалната поява на обекта във форма  //featureBufferT.set_Value(T_CreateDate, splitstringspace[6]);
	string endDate; //  е дата на преустановяване на легалното съществуване обекта  //featureBufferT.set_Value(T_EndDate, splitstringspace[7]);
	double rotAngle; //  е ъгъл на завъртане на текста(100 гона за хоризонтален)  //featureBufferT.set_Value(T_RotAngle, System.Convert.ToDouble(splitstringspace[8]));
	string alignment;// двубуквен код за подравняване   //featureBufferT.set_Value(T_Alignment, splitstringspace[9]);
	string textNadpis;
};

struct structCadasterLayers
{
	std::map<int, std::string> cadasterLines;
	std::map<std::string, std::string> cadasterSgradi;
	std::map<std::string, structConturAttr> conturSgradiAttr;
	std::map<std::string, std::string> cadasterImoti;
	std::map<std::string, structConturAttr> conturImotiAttr;
	std::map<std::string, structNadpisiAtrr> nadpisiAttr;
	std::map<std::string, std::string> cadasterGranici;
	std::map<std::string, structConturAttr> conturGraniciAttr;

};

struct structLayerShemi
{
	std::map<std::string, std::string> shemiAparts;
	std::map<std::string, structConturAttr> conturShemiAttr;
};

struct structField
{
	string name;
	OGRFieldType ogrType;
	int lentgh;
	int flag; //1 - key, 2 -  поле за връзка с класификатор, идентичен с името на полето name, 3 - поле за връзка с таблица и име на поле name за вход в нея
	string relTable; // name relation table, "name" is the key in this table
};

struct structFieldShape
{
	//structFieldShape() : name(), ogrType(), lentgh(), table() {}
	//structFieldShape(std::string newName, OGRFieldType newogrType, int newlentgh, string newtable)
	//	: name(newName), ogrType(newogrType), lentgh (newlentgh), table (newtable) {}
	string name;
	OGRFieldType ogrType;
	int lentgh;
	string table;
};

struct structDataPOZEMLIMOTI
{
////F name	type	Len	dec	flag	table	Име на полето
	string IDENT;	//C	20	0	1		Идентификатор на имот
	int VIDT;		//S	1	0	2		Вид територия
	int VIDTOLD;		//S	1	0	2		Вид територия(стара номенклатура)
	int VIDS;		//S	2	0	2		Вид собственост
	int NTP;		//S	4	0	2		Начин на трайно ползване
	int NTPOLD;		//S	4	0	2		Начин на трайно ползване(стара номенклатура)
	int MESTNOST;	//S	4	0	3	MESTNOSTI	Код на местност
	string PARTIDA;	//C	20	0			Номер на партида от ИР
	int ADDRCODE;	//L	10	0	3	ADDRESS	Адрес на имота
	string NOMER1;	//C	10	0			Стар планоснимачен номер
	string KVARTAL;	//C	10	0			Номер на квартал от регулационен план
	string PARCEL; 	//C	10	0			Номер на парцел  от регулационен план с римски цифри
	double GODCAD;	//N	4	0			Година на стария кадастрален план
	double GODREG;	//N	4	0			Година на стария регулационен план
	int CODZAP;		//S	4	0	3	ZAPOVEDI	Код на последната заповед за одобряване
	int ZACON;		//S	2	0	2		Код на закон, по който са установени границите
	int KAT;			//S	2	0			Преобладаваща категория на земята
	int NVAST;		//S	1	0	2		Начин на възстановяване - от класификатора за начините на възстановяване
	int VAVOD;		//B	1	0			флаг за въвод във владение
	string BEG_DATE;	//D	10	0			Дата на регистрация
	string END_DATE;	//D	10	0			Дата на отрегистрация
};

struct structDataSGRADI
{
	////F name	type	Len	dec	flag	table	Име на полето
	string IDENT;	//C	20	0	1		Идентификатор на сграда
	int VIDS;		//S	2	0	2		Вид собственост
	int CONST;		//S	2	0	2		Конструкция на сградата
	int VFUNC;		//S	3	0	2		Функционално предназначение на сградата
	int VFUNCOLD;	//S	3	0	2		Функционално предназначение на сградата –(стара номенклатура)
	int GOD;		//S	4	0			Година на построяване на сградата
	string PARTIDA;	//C	20	0			Номер на партида от ИР
	string NOMER1;	//C	10	0			Стар планоснимачен номер
	int ЕТ;			//S	3	0			Брой етажи на сградата
	int ЕТ1;		//S	3	0			Брой допълнителни етажи
	int BRPOM;		//S	3	0			Брой самостоятелни обекти
	int ADDRCODE;	//L	10	0	3	ADDRESS	Адрес – код
	int LEGAL;		//S	1	0			Флаг за законност : 0 - няма данни,	1 - незаконна, 2 – законна
	string LEGALDOC;//C	20	0			Описание на документите за законност
	string BEG_DATE;//D	10	0			Дата на регистрация
	string END_DATE;//D	10	0			Дата на отрегистрация
};

struct structDataPRAVA
{
	////F name	type	Len	dec	flag	table	Име на полето
	string IDENT;	//C	20	0	1		Идентификатор на поземлен имот, сграда или самостоятелен обект в сграда
	int VIDS;		//S	2	0	2		Вид собственост
	string PERSON;	//C	13	0	3	PERSONS	ЕГН / Булстат на носителя на правото
	int DOCCOD;		//L	10	0	3	DOCS	Код на документ
	int DOCID1;	    //S	3	0			Идеална част : стойност на числителя, -1 при проценти, -2 при площ в кв.м.
	double DOCID2;	//N	8	3			Идеална част : знаменател, число на процента или площта в кв.м.площ(в зависимост от DOCID1)
	double PLDOC;	//N	11	3			Площ по документ
	int PTYPE;		//S	2	0	2		Начин на придобиване
	int PRAVOVID;	//S	2	0	2		Код на вида право
	string SROK;	//D	10	0			Крайна дата на правото когато правото е срочно
	string DOCIDENT;	//C	30	0			Номер на обекта по документ - свободен текст, например квартал и парцел, пл.сним.номер и т.н.
	int DOP;		//B	1	0			Флаг за допълнителнителен документ(собственост); Т – допълнително, F – основен документ
	string BEG_DATE;	//D	10	0			Дата на регистрация на правото
	string END_DATE;	//D	10	0			Дата на прекратяване на правото
	string END_TIME;	//T	5	0			Време(момент) на прекратяване на правото
};

struct structDataPERSONS
{
	////F name	type	Len	dec	flag	table	Име на полето
	string PERSON;	// C	13	0	1		ЕГН / БУЛСТАТ на соб¬стве¬ник(не се въвежда буква в кода по БУЛСТАТ)
	int SUBTYPE;	// S	1	0	2		Вид на субекта, код от номенклатура
	string NAME;	// C	45	0			Име на собственик
	string NSTATE;	// C	2	0	2		Адрес на собственика - буквен код на страна
	int ADDRCODE;	// L	10	0	3	ADDRESS	Адрес на собственика - код на адрес
	string ADDR;	// C	50	0			Пълен адрес на собственика - коментар
	string ADDRET;	// C	4	0			Адрес на собственика – етаж
	string ADDRAP;	// C	4	0			Адрес на собственика – апартамент
	int FLAG;	    // B	1	0			Флаг за починал собственик
	string SPERSON;	// C	10	0			ЕГН на свързано лице(съпруг / съпруга или физическо лице за ЕТ)
	string FIRMREG;	// C	50	0			Данни за съдебна регистрация на фирма
	string BEG_DATE;// D	10	0			Дата на регистрация
	string END_DATE;// D	10	0			Дата на отрегистрация
};

struct structDataAPARTS
{
	////F name	type	Len	dec	flag	table	Име на полето
	string IDENT;   //	C	20	0	1		Идентификатор на самостоятелен обект
	string REM;	    //  C	20	0			Описание на обекта
	int PREDN;	    //  S	3	0	2		Предназначение на самостоятелен обект
	int VIDS;	    //  S	2	0	2		Вид собственост
	string PARTIDA;	//  C	20	0			Номер на партида от ИР
	int ADDRCODE;	//  L	10	0	3	ADDRESS	Адрес – код
	string ADDRET;	//  C	4	0			Адрес – етаж
	string ADDRAP;  //	C	4	0			Адрес – апартамент
	double PLDOC;	//  N	11	3			Площ по документ
	int BRET;	    //  S	2	0			Брой етажи на обекта
	string DOPS;	//  C	80	0			Вид и площ на прилежащите помещения и общите части, които са неразделна част от обекта(свободен текст)
	string BEG_DATE;//  D	10	0			Дата на регистрация
	string END_DATE;//  D	10	0			Дата на отрегистрация
};

struct structDataOGRPIMO
{
	////F name	type	Len	dec	flag	table	Име на полето
	string IDENT;	//F IDENT    C  20 0 3 POZEMLIMOTI
	int TYPE;     //S   3 0 2
	int DOCCOD;   //L  10 0 3 DOCS
	int DOCCOD1;  //L  10 0 3 DOCS
	int STATUS;   //S   1 0 2
	string BEG_DATE;	//D	10	0			Дата на регистрация
	string END_DATE;	//D	10	0			Дата на отрегистрация
};

struct structDataMESTNOSTI
{
	////F name	type	Len	dec	flag	table	Име на полето
	int MESTNOST;	//S	4	0	3	MESTNOSTI	Код на местност
	string NAME;	//C	20	0			Номер на партида от ИР
	string BEG_DATE;	//D	10	0			Дата на регистрация
	string END_DATE;	//D	10	0			Дата на отрегистрация
};

struct structAttrTables
{
	std::map<std::string, std::vector<structField>> tableFields;
	std::map<std::string, std::multimap<std::string, std::string>> tableAttributes;
	std::map<std::string, structDataPOZEMLIMOTI> tableAttributesPOZEMLIMOTI;
	std::map<std::string, structDataSGRADI> tableAttributesSGRADI;
	std::map<int, structDataMESTNOSTI> tableAttributesMESTNOSTI;
	std::multimap<std::string, structDataPRAVA> tableAttributesPRAVA;
	std::map<std::string, structDataPERSONS> tableAttributesPERSONS;
	std::map<std::string, structDataAPARTS> tableAttributesAPARTS;
	std::multimap<std::string, structDataOGRPIMO> tableAttributesOGRPIMO;
};

struct structNomTABLEVIDS
{
	int VIDS;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string VIDS_txt; // C 30 0 2
};

struct structNomTABLEVIDT
{
	int VIDT;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string VIDT_txt; // C 30 0 2
};
struct structNomTABLENTP
{
	int NTP;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string NTP_txt; // C 30 0 2
};

struct structNomTABLEVFUNC
{
	int VFUNC;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string VFUNC_txt; // C 30 0 2
};
struct structNomTABLEPREDN
{
	int PREDN;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string PREDN_txt; // C 30 0 2
};
struct structNomTABLEOGRPIMO_TYPE
{
	int TYPE;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string TYPE_txt; // C 30 0 2
};

struct structNomTABLENADPISI
{
	int TYPE;		//S	2	0	2		Вид собственост	F VIDS S 2 0 2
	string TYPE_txt; // C 30 0 2
};

struct structNomenclatureTables
{
	std::map<std::string, std::vector<structField>> tableFields;
	std::map<std::string, std::map<std::string, std::string>> tableNomenclaturеs;
	std::map<int, structNomTABLEVIDS> tableNomenclaturеsVIDS;
	std::map<int, structNomTABLEVIDT> tableNomenclaturеsVIDT;
	std::map<int, structNomTABLENTP> tableNomenclaturеsNTP;
	std::map<int, structNomTABLEVFUNC> tableNomenclaturеsVFUNC;
	std::map<int, structNomTABLEPREDN> tableNomenclaturеsPREDN;
	std::map<int, structNomTABLEOGRPIMO_TYPE> tableNomenclaturеsOGRPIMO_TYPE;
	std::map<int, structNomTABLENADPISI> tableNomenclaturеsNADPISI;
};

structCharArray readFile(const char* filename);

void populateLayerShemi(structCharArray &charArrayFromFile, structLayerShemi &layerShemi);

void populateHeader(structCharArray &charArrayFromFile, structReference  &reference, std::string nameShapeFile);


void populateCadasterLayers(structCharArray &charArrayFromFile, structCadasterLayers  &cadasterLayers);

void populateAttributeTable(structCharArray &charArrayFromFile, structAttrTables &attrTables, string tableName);


int populateNomenclatureTables(structNomenclatureTables &nomTables);

void populateCADEntyties(structCharArray &,
	structReference &,
	structCadasterLayers &,
	structLayerShemi &,
	structAttrTables &,
	std::string
	);


#endif