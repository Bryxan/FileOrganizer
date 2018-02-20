#include "stdafx.h"
#include <regex>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;

string CleanFileTitle(string fileName);
void RenameFile(string filePath);
void RenameFilesInFolder(string folderPath, bool recursive);
bool IsTitleClean(string fileName);


int main()
{
	RenameFilesInFolder("A:\\Téléchargements", false);
	return 0;
}

/// <summary>
/// Renames all the files in a folder.
/// </summary>
/// <param name="folderPath">The target folder path.</param>
/// <param name="recursive">if set to <c>true</c> [recursive].</param>
void RenameFilesInFolder(string folderPath, bool recursive)
{
	string extension = string(".mkv");
	for (auto & p : fs::directory_iterator(folderPath))
	{
		std::experimental::filesystem::directory_entry dir = p;
		if (dir.path().has_extension())
		{
			if (dir.path().extension() == extension)
			{
				if (!IsTitleClean(dir.path().string()))
				{
					RenameFile(dir.path().string());
				}
			}
		}
		else 
		{
			cout << p << endl;
		}
	}
}


/// <summary>
/// Renames the file.
/// </summary>
/// <param name="filePath">The file path of the file to rename.</param>
void RenameFile(string filePath)
{
	string originaleName = filePath.substr(filePath.find_last_of("/\\") + 1);
	string cleanName = CleanFileTitle(originaleName);
	string cleanPath = filePath.substr(0, filePath.find_last_of("/\\") + 1) + cleanName;
	rename(filePath.c_str(), cleanPath.c_str());
}

/// <summary>
/// Determines whether the fileName name is already cleared.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns>
///   <c>true</c> if the fileName name is already cleared; otherwise, <c>false</c>.
/// </returns>
bool IsTitleClean(string fileName)
{
	smatch match_result;
	regex r("[[:digit:]]+p");
	string quality;

	// Quality
	if (regex_search(fileName, match_result, r))
	{
		for (auto x : match_result)
		{
			quality = x;
		}
	}

	if (quality.empty())
	{
		return true;
	}
	return false;
}


/// <summary>
/// Cleans the file title.
/// </summary>
/// <param name="fileName">Name of the file to clean.</param>
/// <returns>Cleaned file title</returns>
string CleanFileTitle(string fileName)
{
	string extension = fileName.substr(fileName.find_last_of("."));
	string showName;
	string episode;
	string season;
	string quality;

	smatch match_result;
	regex r("[[:digit:]]+p");

	// Quality
	if (regex_search(fileName, match_result, r))
	{
		for (auto x : match_result)
		{
			quality = x;
		}
	}

	r = ("S[[:digit:]]+");

	// Season
	if (regex_search(fileName, match_result, r))
	{
		for (auto x : match_result)
		{
			season = x;
		}
	}

	r = ("E[[:digit:]]+|[[:space:]][[:digit:]]+[[:space:]]");

	// Episode
	if (regex_search(fileName, match_result, r))
	{
		for (auto x : match_result)
		{
			episode = x;
		}
	}

	r = ("(.+?).S[[:digit:]]|[[:space:]](.+?)[[:space:]]-");

	// Title
	if (regex_search(fileName, match_result, r))
	{
		for (auto x : match_result)
		{
			if (x != "")
			{
				showName = x;
			}
		}
	}

	cout << "Name : " + showName << endl;
	cout << "Episode : " + episode << endl;
	cout << "Season : " + season << endl;
	cout << "Quality : " + quality << endl;

	string finalName;
	// + " [" + quality + "]"
	if (season != "")
	{
		finalName = showName + " " + season + episode;
	}
	else 
	{
		finalName = showName + " "  + episode;
	}

	return finalName + extension;
}

