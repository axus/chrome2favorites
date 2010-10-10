//////////////////////////////////////////////////////////////////////////
//
// chrome class is provided to export google
// chrome bookmarks to IE favorites
//
// by 365days.enjoy#gmail.com 
//   & alan.ncu#gmail.com
//                                                 2010.4.14
// axus.buy#gmail.com                              2010.10.2

#ifndef GOOGLECHROMEUTIL_H
#define GOOGLECHROMEUTIL_H

#include "json/json.h"
#include <string>
#include "sharefile.h"


bool mk_url_obj(std::string id, std::string name, std::string url, Json::Value& obj);

// construct an empty folder object.
bool mk_folder_obj(std::string id, std::string name, Json::Value& obj);

class googleChromeUtil
{
public:
	googleChromeUtil() {};

	///////////////////////////////////////////////////////////////////
	//
	// load
	//
	// Purpose:     load a google chrome bookmarks file.
	//
	// Parameters:  const wchar_t *       --- the file path of chrome bookmarks.
	//
	// Returns:   bool
	//
	bool load(const wchar_t * file_path);


	///////////////////////////////////////////////////////////////////
	//
	// export_2_folder
	//
	// Purpose:     this function is provided to export all bookmarks to a folder
	//
	// Parameters:  const wchar_t* folder_path     --- the folder path
	//
	// Returns:   
	//
	bool export_2_folder(const wchar_t* folder_path);



	///////////////////////////////////////////////////////////////////
	//
	// is_installed_chrome
	//
	// Purpose:     to determine if chrome is installed in current OS
	//
	// Parameters: wchar_t *     --- if chrome is installed, the parameter get the file path. 
	//                                                it's the user's responsibility to allocate space, for example:   wchar_t path_out[260]={0};
	//
	// Returns:   bool
	//
    static bool is_installed_chrome(wchar_t * path);

	static bool is_chrome_running();
	static bool close_chrome();


private:

//////////////////////////////////////////////////////////////////////////
	// this two classes are only provided to use in the class member functions.

	///////////////////////////////////////////////////////////////////
	//
	// export_url
	//
	// Purpose:     export an url to a specific folder path.
	//
	// Parameters: Json::Value&       ---  it must be an url object value. 
	//                       const wchar_t*   ---  the folder path to export.
	//
	// Returns:   void
	//
	void export_url(Json::Value& url_obj, const wchar_t* path);

	///////////////////////////////////////////////////////////////////
	//
	// export_url
	//
	// Purpose:     export a folder object to a specific folder path.
	//
	// Parameters: Json::Value&       ---  it must be an folder object value. 
	//                       const wchar_t*   ---  the folder path to export.
	//
	// Returns:   void
	//
	void export_folder(Json::Value& folder_obj, const wchar_t* path);

	Json::Value chrome_;
};





#endif