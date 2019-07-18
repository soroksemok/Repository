#include <fstream>
#include "Website.h"
#include <string>
#include <urlmon.h>
#include <iostream>




HRESULT Website::DownloadHTML()
{
	HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), "result.html", 0, NULL);
	return hr;
}



Website::Website(std::string name)
{
	url = name;
	texthtml = "";
	textfinal = "";
}

void deleteHTTP(std::string &urll)
{
	int pos = urll.find("//www.");
	if (pos > -1) urll = urll.substr(pos + 6);
	else {
		pos = urll.find("//"); urll = urll.substr(pos + 2);
	}
}

void deleteQMark(std::string &url1)
{
	int posqmark = url1.find("?");
	if (posqmark > -1)
	{
		url1.erase(posqmark, url1.size() - posqmark + 1);
	}
}

std::string setNameTxtFile(std::string &url1)
{
	std::string NameTxtFile = "index.txt";
	int posdot = url1.rfind(".");
	int poslastslash = url1.rfind("/");
	if (posdot > poslastslash)
	{
		NameTxtFile = url1.substr(poslastslash + 1, posdot - poslastslash) + "txt";
		url1.erase(poslastslash + 1, url1.size() - poslastslash);
	}
	return NameTxtFile;
}

void SlashesToBackslashes(std::string &url1)
{
	int posslash = url1.find("/");
	while (posslash > -1)
	{
		url1.replace(posslash, 1, "\\");
		posslash = url1.find("/");
	}
	int poslastslash = url1.rfind("\\");
	if (poslastslash < url1.size() - 1)
		url1.insert(url1.size() - 1, "\\");
}

std::string CurrentDirectory()
{
	TCHAR name[MAX_PATH];
	GetModuleFileName(GetModuleHandle(0), name, MAX_PATH);
	int i = 0;
	int j;
	while (name[i] != '\0')
	{
		if (name[i] == '\\') j = i;
		i++;
	}
	name[j + 1] = '\0';
	std::string namecur = name;
	return namecur;
}

void Website::DeleteTags()
{
	int pos1, pos2;
	pos1 = textfinal.find("<");
	pos2 = textfinal.find(">", pos1);
	while (pos1 > -1 && pos2 > -1)
	{
		textfinal.erase(pos1, pos2 - pos1+1);
		pos1 = textfinal.find("<");
		pos2 = textfinal.find(">", pos1);
	}
}

void CreateFolders(std::string url1,std::string &curdir)
{
	int posslashurl1 = 0;
	int posslashurl2 = url1.find("\\");
	curdir += url1.substr(0, posslashurl2);
	CreateDirectory(curdir.c_str(), NULL);
	posslashurl1 = posslashurl2;
	posslashurl2 = url1.find("\\", posslashurl1 + 1);
	while (posslashurl2 > -1)
	{
		curdir += url1.substr(posslashurl1, posslashurl2 - posslashurl1);
		CreateDirectory(curdir.c_str(), NULL);
		posslashurl1 = posslashurl2;
		posslashurl2 = url1.find("\\", posslashurl1 + 1);
	}
}
void Website::Write()
{
		std::string url1 = url;
		deleteHTTP(url1);
		deleteQMark(url1);
		std::string NameTxtFile = setNameTxtFile(url1);
		SlashesToBackslashes(url1);
		std::string curdir = CurrentDirectory();
		CreateFolders(url1,curdir);
		std::ofstream file(curdir+"\\"+NameTxtFile);
		file << textfinal;
}

void Website::FindTag(std::string tag)
{
	int pos1, pos2,pos11, pos3=-1,pos4=-1;
	pos1 = texthtml.find("<"+tag+">");
	pos2 = texthtml.find("</"+tag+">");
	pos11 = texthtml.find("<" + tag + " ");
	if (pos11 > -1 && (pos11 < pos1 || pos1 == -1)) pos1 = pos11;
	if (pos1 > -1)
	{
		pos3 = texthtml.find("<" + tag + ">", pos1 + tag.size() + 2);
		pos4 = texthtml.find("<" + tag + " ", pos1 + tag.size() + 2);
		if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
	}
	while (pos3 > -1 && pos3 < pos2)
	{
		pos2 = texthtml.find("</" + tag + ">", pos2 + tag.size() + 3);
		pos4 = texthtml.find("<" + tag + " ", pos3 + tag.size() + 2);
		pos3 = texthtml.find("<" + tag + ">", pos3 + tag.size() + 2);
		
		if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
	}
	while (pos1 > -1 && pos2 > -1)
	{
		pos3 = -1; pos4 = -1;
		textfinal = textfinal + texthtml.substr(texthtml.find(">", pos1) + 1, pos2 - texthtml.find(">", pos1) - 1) + '\n' + '\n';
		texthtml.erase(pos1, pos2 - pos1 + 3 + tag.size());
		pos1 = texthtml.find("<" + tag + ">");
		pos2 = texthtml.find("</" + tag + ">");
		pos11 = texthtml.find("<" + tag + " ");
		if (pos11 > -1 && (pos11 < pos1 || pos1 == -1)) pos1 = pos11;
		if (pos1 > -1)
		{
			pos3 = texthtml.find("<" + tag + ">", pos1 + tag.size() + 2);
			pos4 = texthtml.find("<" + tag + " ", pos1 + tag.size() + 2);
			if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
		}
		while (pos3 > -1 && pos3 < pos2)
		{
			pos2 = texthtml.find("</" + tag + ">", pos2 + tag.size() + 3);
			pos4 = texthtml.find("<" + tag + " ", pos3 + tag.size() + 2);
			pos3 = texthtml.find("<" + tag + ">", pos3 + tag.size() + 2);
			
			if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
		}
	}
}

/*void Website::DeleteTag(std::string tag)
{
	int pos1, pos2, pos11, pos3 = -1, pos4 = -1;
	pos1 = textfinal.find("<" + tag + ">");
	pos2 = textfinal.find("</" + tag + ">");
	pos11 = textfinal.find("<" + tag + " ");
	if (pos11 > -1 && (pos11 < pos1 || pos1 == -1)) pos1 = pos11;
	if (pos1 > -1)
	{
		pos3 = textfinal.find("<" + tag + ">", pos1 + tag.size() + 2);
		pos4 = textfinal.find("<" + tag + " ", pos1 + tag.size() + 2);
		if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
	}
	while (pos3 > -1 && pos3 < pos2)
	{
		pos2 = textfinal.find("</" + tag + ">", pos2 + tag.size() + 3);
		pos4 = textfinal.find("<" + tag + " ", pos3 + tag.size() + 2);
		pos3 = textfinal.find("<" + tag + ">", pos3 + tag.size() + 2);
		
		if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
	}
	while (pos1 > -1 && pos2 > -1)
	{
		pos3 = -1; pos4 = -1;
		textfinal.erase(pos1, pos2 - pos1 + 3 + tag.size());
		pos1 = textfinal.find("<" + tag + ">");
		pos2 = textfinal.find("</" + tag + ">");
		pos11 = textfinal.find("<" + tag + " ");
		if (pos11 > -1 && (pos11 < pos1 || pos1 == -1)) pos1 = pos11;
		if (pos1 > -1)
		{
			pos3 = textfinal.find("<" + tag + ">", pos1 + tag.size() + 2);
			pos4 = textfinal.find("<" + tag + " ", pos1 + tag.size() + 2);
			if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
		}
		while (pos3 > -1 && pos3 < pos2)
		{
			pos2 = textfinal.find("</" + tag + ">", pos2 + tag.size() + 3);
			pos4 = textfinal.find("<" + tag + " ", pos3 + tag.size() + 2);
			pos3 = textfinal.find("<" + tag + ">", pos3 + tag.size() + 2);
			
			if ((pos4<pos3 && pos4 > -1) || (pos3 == -1)) pos3 = pos4;
		}
	}
}*/

std::string Utf8_to_cp1251(const char *str)
{
	std::string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);

	if (!result_u)
		return 0;

	wchar_t *ures = new wchar_t[result_u];

	if (!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return 0;
	}

	result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);

	if (!result_c)
	{
		delete[] ures;
		return 0;
	}

	char *cres = new char[result_c];

	if (!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
	{
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}

void Website::FileToString()
{
	std::fstream file("result.html");
	std::getline(file, texthtml, '\0');
	if (texthtml.find("charset=windows-1251") == -1)
		texthtml = Utf8_to_cp1251(texthtml.c_str());
	file.close();
	remove("result.html");
}

void Website::DeleteEmptyLines()
{
	int pos = textfinal.find("\n \n");
	while (pos > -1)
	{
		textfinal.erase(pos, 1);
		pos = textfinal.find("\n \n");
	}
	pos = textfinal.find("\n\n\n");
	while (pos > -1)
	{
		textfinal.erase(pos, 1);
		pos = textfinal.find("\n\n\n");
	}

}
void Website::DeleteSymbols()
{
	int pos1,pos2;
	pos1 = textfinal.find("&");
	while (pos1 > -1)
	{
		pos2 = textfinal.find(";", pos1);
		if (textfinal[pos1 + 1] == 'n' && textfinal[pos1 + 2] == 'b')
		{
			textfinal.erase(pos1, pos2 - pos1+1);
			textfinal.insert(pos1, " ");
			pos1 = textfinal.find("&", pos1);
			continue;
		}
		if (pos2 > -1 && pos2 - pos1 < 10)
		{
			textfinal.erase(pos1, pos2 - pos1 + 1);
			pos1 = textfinal.find("&", pos1);
		}
		else
		{
			pos1 = textfinal.find("&", pos1 + 1);
		}
	}
}

void Website::ShowLinks()
{
	int pos1, pos2;
	std::string link;
	pos1 = textfinal.find("<a ");
	pos2 = textfinal.find("</a>");
	while (pos1 > -1 && pos2 > -1)
	{
		int FindLink= textfinal.find("href=", pos1) + 6;
		link = textfinal.substr(FindLink, textfinal.find(textfinal[FindLink-1], FindLink) - FindLink);
		if (link.find("http") != 0 && link.find("//") != 0)
		{
			int posurl = url.find("/", 9);
			if (posurl == -1) posurl = url.size();
				link = url.substr(0, posurl)+link;
		}
		textfinal.erase(pos2, 4);
		textfinal.insert(pos2, " [" + link + "]");
		int tmp = textfinal.find(">",pos1);
		textfinal.erase(pos1, textfinal.find(">",pos1) - pos1 +1);
		pos1 = textfinal.find("<a ");
		pos2 = textfinal.find("</a>");
	}
}

void Website::Format()
{
	std::fstream config("config.ini",std::ios::in|std::ios::out|std::ios::app);
	std::string textconfig;
	std::getline(config, textconfig, '\0');
	int width;
	int poswidth = textconfig.find("width=");
	int posnext = textconfig.find("\n", poswidth);
	if (posnext == -1) posnext = textconfig.size();
	if (poswidth == -1) width = 80;
	else width = std::atoi(textconfig.substr(poswidth + 6, posnext - poswidth + 6).c_str());
	if (width <= 0) width = 80;
	int pos1=textfinal.find("\n");
	int pos2 = textfinal.find("\n", pos1 + 1);
	int space, tempspace;
	while (pos1 > -1)
	{
		if (pos2 - pos1 > width)
		{
			if (pos2 == -1) pos2 = textfinal.size();
			space = textfinal.rfind(" ", pos1 + width);
			if (space == -1) space = pos1 + width;
			textfinal.insert(space + 1, "\n");
			pos2++;
			while (pos2 - space > width)
			{
				tempspace = space;
				space = textfinal.rfind(" ", space + width);
				if (space <= tempspace) space = tempspace + width;
				textfinal.insert(space + 1, "\n");
				pos2++;
			}
		}
		 pos1 = textfinal.find("\n",pos2);
		 pos2 = textfinal.find("\n", pos1 + 1);
	}
}

/*void Website::DeleteTag2(std::string tag)
{
	int pos = textfinal.find("<"+tag);
	while (pos > -1)
	{
		textfinal.erase(pos, textfinal.find("/>", pos) - pos + 2);
		pos = textfinal.find("<"+tag);
	}
}*/

void Website::DeleteEmptySpaces()
{
	int pos;
	pos = textfinal.find("  ");
	while (pos > -1)
	{
		textfinal.erase(pos, 1);
		pos = textfinal.find("  ");
	}
}

void Website::ReplaceBr()
{
	int pos;
	pos = textfinal.find("<br>");
	while (pos > -1)
	{
		textfinal.replace(pos, 4, "\n");
		pos = textfinal.find("<br>");
	}
}


Website::~Website()
{
}
