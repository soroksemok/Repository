#include <iostream>
#include "Website.h"
#include <conio.h>
#include <fstream>


int main(int argc, char* argv[])
{
	std::string url = argv[1];
	Website website(url);

	HRESULT hr= website.DownloadHTML();

	if ( hr == S_OK)
	{
		website.FileToString();
		website.FindTag("title");
		website.FindTag("p");
		website.DeleteSymbols();
		website.ShowLinks();
		website.ReplaceBr();
		website.DeleteTags();
		website.DeleteEmptySpaces();
		website.DeleteEmptyLines();
		website.Format();
		website.Write();
	}
	else
	{
		std::cout << "DownloadHTML failed with error code: " << hr << "\n";
	}
	return 0;
}






