#include "language.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Language::Language()
{
	//Set the default language
	current = "en";
}

Language::~Language()
{
	strings.clear();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Languages
//----------------------------------------------------------------------------------------------------

std::wstring Language::getLanguageName(std::string languageID)
{
	return languageNames[languageID];
}

std::wstring Language::currentName()
{
	return languageNames[current];
}

void Language::setLanguage(std::string languageID)
{
}

void Language::loadLanguages()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Strings
//----------------------------------------------------------------------------------------------------

std::wstring Language::getString(std::string name)
{
	return L"<unknown>";
}

void Language::loadStrings()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void Language::eventPrepare()
{
	//TODO: load the language settings from the user defaults...
	
	//Load the languages available on disk
	loadLanguages();
	
	//Load the language strings
	loadStrings();
}
