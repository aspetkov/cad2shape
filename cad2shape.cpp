#include "stdafx.h"
#include <iostream>  
#include <fstream>  
#include <string>
#include <cstring>
#include <stdlib.h>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <vector>
#include <iterator>
#include <map>
#include <sstream>
#include <chrono> 

#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileImoti.h"
#include "shapefileSgradi.h"
#include "shapefilePrava.h"
#include "shapefileShemi.h"
#include "shapefileOgrpimo.h"
#include "shapefileNadpisi.h"
#include "helper.h"
#include "helper_template.cpp"
#include <boost/filesystem.hpp>
#include <Windows.h>


using namespace boost::filesystem;
using namespace std;


int main (int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода


	cout << "Copyright(c) 2019 Asen Asenov" << '\n';
	cout << "Email: aspetkov@abv.bg" << '\n' << '\n';
	cout << "The MIT License(MIT)" << '\n';
	cout << "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR" << '\n';
	cout << "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS" << '\n';
	cout << "FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR" << '\n';
	cout << "COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER" << '\n';
	cout << "IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN" << '\n';
	cout << "CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." << '\n' << '\n';

	Sleep(2000);
	
	string dirName = argv[1];
	if (dirName == "")
	{
		cout << "Enter file name" << endl;
		cout << "cad2shape.exe filename" << endl;
		return 0;
	}

	if (argc < 2)
	{
		cout << "Usage: tut4 path\n";
		return 1;
	}

	path p(dirName);
	dirName += '\\';
	std::vector<string> cadFiles;
	try
	{
		if (exists(p))
		{
			if (is_regular_file(p))
			{
				cout << p << " size is " << file_size(p) << '\n';
				cadFiles.push_back(p.string());
			}

			else if (is_directory(p))
			{
				cout << p << " is a directory containing:\n";

				std::vector<path> v;

				for (auto&& x : directory_iterator(p))
					v.push_back(x.path());

				std::sort(v.begin(), v.end());

				for (auto&& x : v)
				if (x.extension() == ".cad")
				{
					std::string s = x.filename().string();
					cadFiles.push_back(dirName+s);
					cout << "    " << x.filename() << '\n';
				}
			}
			else
				cout << p << " exists, but is not a regular file or directory\n";
		}
		else
			cout << p << " does not exist\n";
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << '\n';
	}

	
	structNomenclatureTables nomTables;
	populateNomenclatureTables(nomTables);

	for (auto&& x : cadFiles)
	{
		cout << "    " << x  << '\n';
		structCadasterLayers cadasterLayers;
		structAttrTables attrTables;
		structLayerShemi layerShemi;

		structReference reference;
		string nameShapeFile = x;
		const char * filename = nameShapeFile.c_str();
		structCharArray charArrayFromFile = readFile(filename);

//		Remove extension if present.
		const size_t period_idx = nameShapeFile.rfind('.');
		if (std::string::npos != period_idx)
		{
			nameShapeFile.erase(period_idx);
		}
		nameShapeFile = ConvertCyrCharToLatin(nameShapeFile);
		populateCADEntyties(charArrayFromFile,
			reference,
			cadasterLayers,
			layerShemi,
			attrTables,
			nameShapeFile);

		ShapefileImoti ShapefileImotiObject(nameShapeFile,
			cadasterLayers,
			attrTables,
			nomTables,
			reference);

		ShapefileImotiObject.createShapefileIMOTI();

		ShapefileSgradi ShapefileSgradiObject(nameShapeFile,
			cadasterLayers,
			attrTables,
			nomTables,
			reference);

		ShapefileSgradiObject.createShapefileSGRADI();


		ShapefilePrava ShapefilePravaObject(nameShapeFile,
			cadasterLayers,
			layerShemi,
			attrTables,
			nomTables,
			reference);

		ShapefilePravaObject.createShapefilePRAVA("IMOTI");
		ShapefilePravaObject.createShapefilePRAVA("SGRADI");
		ShapefilePravaObject.createShapefilePRAVA("SHEMI");

		ShapefileShemi ShapefileShemiObject(nameShapeFile,
			layerShemi,
			attrTables,
			nomTables,
			reference);
		ShapefileShemiObject.createShapefileSHEMI();

		ShapefileOgrpimo ShapefileOgrpimoObject(nameShapeFile,
			cadasterLayers,
			attrTables,
			nomTables,
			reference);

		ShapefileOgrpimoObject.createShapefile();

		ShapefileNadpisi ShapefileNadpisiObject(nameShapeFile,
			cadasterLayers,
			nomTables,
			reference);
		ShapefileNadpisiObject.createShapefile();
	}

		// Record end time
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::cout << "Elapsed time: " << elapsed.count() << " s\n";
		std::system("pause");

		return 0;

}