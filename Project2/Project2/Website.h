#pragma once
#include <string>
#include <Windows.h>
#include <fstream>
class Website
{
	std::string url;
	std::string texthtml;
	std::string textfinal;

public:
	HRESULT DownloadHTML();
	void FileToString();
	void FindTag(std::string tag);
	void Write();
	//void DeleteTag(std::string tag);
	void DeleteSymbols();
	void ShowLinks();
	void Format();
	//void DeleteTag2(std::string tag);
	void DeleteTags();
	void DeleteEmptyLines();
	void DeleteEmptySpaces();
	void ReplaceBr();
	Website(std::string name);
	~Website();
};

