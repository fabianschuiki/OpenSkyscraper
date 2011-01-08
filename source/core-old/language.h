#ifndef OSS_LANGUAGE_H
#define OSS_LANGUAGE_H

#include "../general.h"
#include "../base/base.h"


namespace OSS {
	class Language : public Singleton<Language, Object> {
	public:
		Language();
		~Language();
		
		/**
		 * Languages
		 */
		
	public:
		//Available Languages
		std::vector<std::string> languages;
		std::wstring getLanguageName(std::string languageID);
		
		//Current Language
		std::string current;
		std::wstring currentName();
		void setLanguage(std::string languageID);
		
	private:
		std::map<std::string, std::wstring> languageNames;
		void loadLanguages();
		
		
		/**
		 * Strings
		 */
		
	public:
		//Translation
		std::wstring getString(std::string name);
		
	private:
		std::map<std::string, std::wstring> strings;
		void loadStrings();
		
		
		/**
		 * Events
		 */
		
	public:
		void eventPrepare();
	};
}


#endif
