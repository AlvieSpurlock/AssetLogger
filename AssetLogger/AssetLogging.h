#pragma once
#include <iostream>
#include <map>
#include <windows.h>
#include <cwchar>
#include <fstream>
#include <sstream>



#include "Console.h"
#include "Input.h"


using namespace std;


class AssetLogging
{
protected:
	bool Demo = false;
	vector<string> assets = {"Platform,Location(in Project),Use(in Project),Asset Name(on Platform),Asset Maker(on Platform),License Type\n"};

	int OpenChances = 3;
	int NewChances = 3;
public:

	void enableANSI() 
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hOut, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}

AssetLogging()
{
	enableANSI();
	Demo;
	assets;
}
void SetConsoleFontSize(int sizeY, const wchar_t* fontName = L"Consolas") {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;        // auto width
	cfi.dwFontSize.Y = sizeY;    // height in pixels
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, fontName);

	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

static void ShowLicense()
{
	Console::SetForegroundColor(Yellow);
	cout << "Copyright © Mad Tea Party Development 2025" << endl
		<< endl
		<< "This software is proprietary and may not be copied, modified, distributed, or used for commercial purposes without explicit permission from the author." << endl
		<< "Demo version provided for evaluation only.Full version available now!"<< endl
		<< endl
		<< endl;
}



void Prompt()
{
	if (Demo)
	{
		SetConsoleFontSize(18);
		Console::SetBackgroundColor(White);
		Console::SetForegroundColor(Black);
		cout << "-----Options-----" << endl;
		Console::SetBackgroundColor(Black);

		cout << endl;


		Console::SetForegroundColor(LightGrey);
		cout << "1:";
		Console::SetForegroundColor(White);
		cout << " New" << endl;
	}
	else
	{
		SetConsoleFontSize(18);
		Console::SetBackgroundColor(White);
		Console::SetForegroundColor(Black);
		cout << "-----Options-----" << endl;
		Console::SetBackgroundColor(Black);

		cout << endl;


		Console::SetForegroundColor(LightGrey);
		cout << "1:";
		Console::SetForegroundColor(White);
		cout << " New" << endl;


		Console::SetForegroundColor(LightGrey);
		cout << "2:";
		Console::SetForegroundColor(White);
		cout << " Update";
	}
}

string MakeAssetInfo(string platform, string assetLocation, string assetUse, string assetName, string assetMaker, string licenseType, char delimiter)
{
	if (Demo)
	{
		return platform + delimiter + assetUse + delimiter + assetMaker + "\n";
	}
	else{ return platform + delimiter + assetLocation + delimiter + assetUse + delimiter + assetName + delimiter + assetMaker + delimiter + licenseType + "\n"; }
	
}

void AddAssetInfo(string assetInfo, vector<string>& assets)
{
	assets.push_back(assetInfo);
}

void UpdateAsset(size_t index, vector<string>& assets)
{
	if (!Demo)
	{
		string platform = Input::GetString("Platform: ", White);
		string assetLocation = Input::GetString("Asset Location(in Project): ", White);
		string assetUse = Input::GetString("Asset Use(in Project): ", White);
		string assetName = Input::GetString("Asset Name(on Platform): ", White);
		string assetMaker = Input::GetString("Asset Maker(on Platform): ", White);
		string licenseType = Input::GetString("License Type: ", White);

		Console::SetForegroundColor(White);
		cout << "\nDo you want me to read it back to you?\n";

		Console::SetForegroundColor(LightGrey);
		cout << "1:";
		Console::SetForegroundColor(White);
		cout << " Yes\n";

		Console::SetForegroundColor(LightGrey);
		cout << "2:";
		Console::SetForegroundColor(Red);
		cout << " No\n\n";

		int readBack = Input::GetInteger("Answer: ", 1, 2, LightGrey);
		if (readBack == 1)
		{
			cout << platform << ',' << assetLocation << ',' << assetUse << ',' << assetName << ',' << assetMaker << ',' << licenseType << endl << endl;

			Console::SetForegroundColor(White);
			cout << "\nIs this correct?\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";
			int correct = Input::GetInteger("Answer:  ", 1, 2, LightGrey);
			if (correct == 1)
			{
				Console::SetForegroundColor(White);
				cout << "\nDo you have more you want to Update?\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";

				int doUpdate = Input::GetInteger("Answer: ", 1, 2, LightGrey);

				if (doUpdate == 1)
				{
					UpdateAsset(index, assets);
				}
				else
				{
					assets[index] = MakeAssetInfo(platform, assetLocation, assetUse, assetName, assetMaker, licenseType, ',');
					SaveAssetInfo(assets, Input::GetString("File Name: ", White) + ".csv");
				}
			}
			else
			{
				Console::SetForegroundColor(White);
				cout << "I'm so sorry to hear that!" << endl
					<< endl;

				Console::SetForegroundColor(White);
				cout << "\nDo you want to try again?\n\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";

				int updateAgain = Input::GetInteger("Answer: ", 1, 2, LightGrey);

				if (updateAgain == 1)
				{
					UpdateAsset(index, assets);
				}
				else
				{
					Console::SetForegroundColor(Yellow);
					cout << "\nAre you sure?\n This would abandon ship!\n\n";

					Console::SetForegroundColor(LightGrey);
					cout << "1:";
					Console::SetForegroundColor(White);
					cout << " Yes\n";

					Console::SetForegroundColor(LightGrey);
					cout << "2:";
					Console::SetForegroundColor(Red);
					cout << " No\n\n";

					int sure = Input::GetInteger("Answer: ", 1, 2, LightGrey);

					if (sure == 1)
					{
						UpdateAsset(index, assets);
					}
					else
					{
						SaveAssetInfo(assets, Input::GetString("File Name: ", White) + ".csv");
					}
				}

			}
		}
	}

	else
	{
		string platform = Input::GetString("Platform: ", White);

		string assetUse = Input::GetString("Asset Use(in Project): ", White);
		
		string assetMaker = Input::GetString("Asset Maker(on Platform): ", White);


		Console::SetForegroundColor(White);
		cout << "\nDo you want me to read it back to you?\n";

		Console::SetForegroundColor(LightGrey);
		cout << "1:";
		Console::SetForegroundColor(White);
		cout << " Yes\n";

		Console::SetForegroundColor(LightGrey);
		cout << "2:";
		Console::SetForegroundColor(Red);
		cout << " No\n\n";

		int readBack = Input::GetInteger("Answer: ", 1, 2, LightGrey);
		if (readBack == 1)
		{
			cout << platform << ',' << assetUse << ',' << assetMaker << endl << endl;

			Console::SetForegroundColor(White);
			cout << "\nIs this correct?\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";
			int correct = Input::GetInteger("Answer:  ", 1, 2, LightGrey);
			if (correct == 1)
			{
				Console::SetForegroundColor(White);
				cout << "\nDo you have more you want to Update?\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";

				int doUpdate = Input::GetInteger("Answer: ", 1, 2, LightGrey);

				if (doUpdate == 1)
				{
					UpdateAsset(index, assets);
				}
				else
				{
					assets[index] = MakeAssetInfo(platform, "Full Version Only", assetUse, "Full Version Only", assetMaker, "Full Version Only", ',');
					SaveAssetInfo(assets, Input::GetString("File Name: ", White) + ".csv");
				}
			}
			else
			{
				Console::SetForegroundColor(White);
				cout << "I'm so sorry to hear that!" << endl
					<< endl;

				Console::SetForegroundColor(White);
				cout << "\nDo you want to try again?\n\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";

				int updateAgain = Input::GetInteger("Answer: ", 1, 2, LightGrey);

				if (updateAgain == 1)
				{
					UpdateAsset(index, assets);
				}
				else
				{
					Console::SetForegroundColor(Yellow);
					cout << "\nAre you sure?\n This would abandon ship!\n\n";

					Console::SetForegroundColor(LightGrey);
					cout << "1:";
					Console::SetForegroundColor(White);
					cout << " Yes\n";

					Console::SetForegroundColor(LightGrey);
					cout << "2:";
					Console::SetForegroundColor(Red);
					cout << " No\n\n";

					int sure = Input::GetInteger("Answer: ", 1, 2, LightGrey);

					if (sure == 1)
					{
						UpdateAsset(index, assets);
					}
					else
					{
						SaveAssetInfo(assets, Input::GetString("File Name: ", White) + ".csv");
					}
				}

			}
		}
	}

	
}

void UpdateAssets(vector<string>& assets, const string& fileName)
{
	ifstream newFile(fileName + ".csv");
	if (!newFile.is_open()) 
	{
		cerr << "Failed to open file: " << fileName << endl;
		return;
	}
	else
	{
		string line;
		assets.clear();
		while (getline(newFile, line))
		{
			if (!line.empty() && line.back() == '\r') { line.pop_back(); }
			assets.push_back(line);
		}

		if (!assets.empty())
		{
			cout << "\n\n\n";
			Console::SetBackgroundColor(White);
			Console::SetForegroundColor(Black);
			cout << "------Results------";
			Console::SetBackgroundColor(Black);
			Console::SetForegroundColor(White);
			cout << "\n\n";

			int index = 0;
			for (auto& asset : assets)
			{
				if (index > 0) { cout << index << ": "; }
				cout << asset << endl;
				index++;
			}

			Console::SetForegroundColor(White);
			cout << "\nWhich one is the problem?\n\n";
			size_t problemIndex = Input::GetInteger("Answer:  ", 0, assets.size() - 1, LightGrey);

			UpdateAsset(problemIndex, assets);
		}
		else
		{
			cout << "-----CODE-BROKEN-----";
		}
	}
}


void SaveAssetInfo(vector<string>& assets, string fileName)
{
	if (!Demo)
	{
	ifstream newFile(fileName);
	string line;

	if (newFile.is_open())
	{
		Console::SetForegroundColor(Yellow);
		cout << "\nFile Already Exists\n" << endl;

		Console::SetForegroundColor(White);
		cout << "\nDo you want to overwrite it?\n";

		Console::SetForegroundColor(LightGrey);
		cout << "1:";
		Console::SetForegroundColor(White);
		cout << " Yes\n";

		Console::SetForegroundColor(LightGrey);
		cout << "2:";
		Console::SetForegroundColor(Red);
		cout << " No\n\n";
		int doOverwrite = Input::GetInteger("Answer:  ", 1, 2, LightGrey);

		if (doOverwrite == 1)
		{
			if (!newFile.eof())
			{
				ofstream file(fileName);
				for (auto& asset : assets)
				{
					file << asset;
				}
				file.close();
			}
		}
		else
		{
			Console::SetForegroundColor(White);
			cout << "\nDo you want to update it?\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";
			int doUpdate = Input::GetInteger("Answer:  ", 1, 2, LightGrey);

			if (doUpdate == 1)
			{
				ifstream newFile(fileName);
				string line;
				while (!newFile.eof())
				{
					getline(newFile, line);

					string assetInfo;
					stringstream assetList(line);

					size_t index = 0;
					while (getline(assetList, assetInfo, '|'))
					{
						if (assets.size() <= index)
						{
							assets[index] = assetInfo;
							index++;
						}
						else
						{
							assets.push_back(assetInfo);
						}
					}
				}

				newFile.close();

				if (!assets.empty())
				{
					cout << "\n \n \n";
					Console::SetBackgroundColor(White);
					Console::SetForegroundColor(Black);
					cout << "------Results------";
					Console::SetBackgroundColor(Black);
					Console::SetForegroundColor(White);
					cout << "\n \n";
					for (auto& asset : assets)
					{
						cout << asset << endl;
					}

					Console::SetForegroundColor(White);
					cout << "\nWhich one is the problem?\n\n";
					size_t problemIndex = Input::GetInteger("Answer:  ", 0, assets.size(), LightGrey);

					UpdateAsset(problemIndex, assets);
				}
				else { cout << "-----CODE-BROKEN-----"; }
			}
		}
	} else{}
	}

	else
	{

			ifstream newFile(fileName);
			string line;
			if (newFile.is_open())
			{
				Console::SetForegroundColor(Yellow);
				cout << "\nFile Already Exists\n" << endl;

				Console::SetForegroundColor(White);
				cout << "\nDo you want to overwrite it?\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";
				int doOverwrite = Input::GetInteger("Answer:  ", 1, 2, LightGrey);

				if (doOverwrite == 1)
				{
					if (!newFile.eof())
					{
						ofstream file(fileName);
						for (auto& asset : assets)
						{
							file << asset;
						}
						file.close();
					}
				}
				else
				{
					Console::SetForegroundColor(White);
					cout << "\nDo you want to update it?\n";

					Console::SetForegroundColor(LightGrey);
					cout << "1:";
					Console::SetForegroundColor(White);
					cout << " Yes\n";

					Console::SetForegroundColor(LightGrey);
					cout << "2:";
					Console::SetForegroundColor(Red);
					cout << " No\n\n";
					int doUpdate = Input::GetInteger("Answer:  ", 1, 2, LightGrey);

					if (doUpdate == 1)
					{
						ifstream newFile(fileName);
						string line;
						while (!newFile.eof())
						{
							getline(newFile, line);

							string assetInfo;
							stringstream assetList(line);

							size_t index = 0;
							while (getline(assetList, assetInfo, '|'))
							{
								if (assets.size() <= index)
								{
									assets[index] = assetInfo;
									index++;
								}
								else
								{
									assets.push_back(assetInfo);
								}
							}
						}

						newFile.close();

						if (!assets.empty())
						{
							cout << "\n \n \n";
							Console::SetBackgroundColor(White);
							Console::SetForegroundColor(Black);
							cout << "------Results------";
							Console::SetBackgroundColor(Black);
							Console::SetForegroundColor(White);
							cout << "\n \n";
							for (auto& asset : assets)
							{
								cout << asset << endl;
							}

							Console::SetForegroundColor(White);
							cout << "\nWhich one is the problem?\n\n";
							size_t problemIndex = Input::GetInteger("Answer:  ", 0, assets.size(), LightGrey) - 1;

							UpdateAsset(problemIndex, assets);
						}
						else { cout << "-----CODE-BROKEN-----"; }
					}
				}
			}
			else
			{
				if (!newFile.eof())
				{
					ofstream file(fileName);
					for (auto& asset : assets)
					{
						file << asset;
					}
					file.close();
				}
			}
	}

	
}

	void ReadBack()
	{
		if (!assets.empty())
		{
			cout << "\n \n \n";
			Console::SetBackgroundColor(White);
			Console::SetForegroundColor(Black);
			cout << "------Results------";
			Console::SetBackgroundColor(Black);
			Console::SetForegroundColor(White);
			cout << "\n \n";
			int index = 1;
			for (size_t assetIndex = 0 ; assetIndex < assets.size() ; assetIndex++)
			{
				if(assetIndex > 0)
				{
					cout << index << ": ";
					index++;
				}
				cout << assets[assetIndex] << endl;
			}

			Console::SetForegroundColor(White);
			cout << "\nIs this correct?\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";
			int correct = Input::GetInteger("Answer:  ", 1, 2, LightGrey);

			if (correct == 1)
			{
				SaveAssetInfo(assets, Input::GetString("File Name: ", White) + ".csv");
			}
			else
			{
				Console::SetForegroundColor(White);
				cout << "I'm so sorry to hear that!" << endl
					<< endl;

				Console::SetForegroundColor(White);
				cout << "\nWhich one is the problem?\n\n";
				size_t problemIndex = Input::GetInteger("Answer:  ", 0, assets.size(), LightGrey) - 1;

				UpdateAsset(problemIndex, assets);
			}

		} else { cout << "-----CODE-BROKEN-----"; }
	}



	void HandleAssetAdding()
	{
		if (!Demo)
		{
			string platform = Input::GetString("Platform: ", White);
			string assetLocation = Input::GetString("Asset Location(in Project): ", White);
			string assetUse = Input::GetString("Asset Use(in Project): ", White);
			string assetName = Input::GetString("Asset Name(on Platform): ", White);
			string assetMaker = Input::GetString("Asset Maker(on Platform): ", White);
			string licenseType = Input::GetString("License Type: ", White);

			Console::SetForegroundColor(White);

			AddAssetInfo(MakeAssetInfo(platform, assetLocation, assetUse, assetName, assetMaker, licenseType, ','), assets);


			Console::SetForegroundColor(White);
			cout << "\nDo you have more Assets?\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";

			int moreToAdd = Input::GetInteger("Answer: ", 1, 2, LightGrey);

			if (moreToAdd == 1)
			{
				HandleAssetAdding();
			}
			else
			{
				Console::SetForegroundColor(White);
				cout << "\nDo you want me to read it back to you?\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";

				int readBack = Input::GetInteger("Answer: ", 1, 2, LightGrey);
				if (readBack == 1)
				{
					ReadBack();
				}
				else
				{
					SaveCheck();
				}
			}
		}

		else
		{
			string platform = Input::GetString("Platform: ", White);

			string assetUse = Input::GetString("Asset Use(in Project): ", White);

			string assetMaker = Input::GetString("Asset Maker(on Platform): ", White);


			Console::SetForegroundColor(White);

			AddAssetInfo(MakeAssetInfo(platform, "Full Version Only", assetUse, "Full Version Only", assetMaker, "Full Version Only", ','), assets);


			Console::SetForegroundColor(White);
			cout << "\nDo you have more Assets?\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";

			int moreToAdd = Input::GetInteger("Answer: ", 1, 2, LightGrey);

			if (moreToAdd == 1)
			{
				HandleAssetAdding();
			}
			else
			{
				Console::SetForegroundColor(White);
				cout << "\nDo you want me to read it back to you?\n";

				Console::SetForegroundColor(LightGrey);
				cout << "1:";
				Console::SetForegroundColor(White);
				cout << " Yes\n";

				Console::SetForegroundColor(LightGrey);
				cout << "2:";
				Console::SetForegroundColor(Red);
				cout << " No\n\n";

				int readBack = Input::GetInteger("Answer: ", 1, 2, LightGrey);
				if (readBack == 1)
				{
					ReadBack();
				}
				else
				{
					SaveCheck();
				}
			}
		}
		
	}

	void SaveCheck()
	{
		Console::SetForegroundColor(White);
		cout << "\nDo you want to save the file?\n";

		Console::SetForegroundColor(LightGrey);
		cout << "1:";
		Console::SetForegroundColor(White);
		cout << " Yes\n";

		Console::SetForegroundColor(LightGrey);
		cout << "2:";
		Console::SetForegroundColor(Red);
		cout << " No\n\n";

		int saveAsk = Input::GetInteger("Answer: ", 1, 2, LightGrey);
		if (saveAsk == 1)
		{
			SaveAssetInfo(assets, Input::GetString("File Name: ", White) + ".csv");
		}
		else
		{
			Console::SetForegroundColor(Yellow);
			cout << "\nAre you sure??\n";

			Console::SetForegroundColor(LightGrey);
			cout << "1:";
			Console::SetForegroundColor(White);
			cout << " Yes\n";

			Console::SetForegroundColor(LightGrey);
			cout << "2:";
			Console::SetForegroundColor(Red);
			cout << " No\n\n";

			int sure = Input::GetInteger("Answer: ", 1, 2, LightGrey);
			if (sure == 1)
			{
				//Do Nothing
			}
			else
			{
				SaveCheck();
			}

		}
	}

	vector<string>& GetAssets() { return assets; }


};
