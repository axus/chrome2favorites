
//////////////////////////////////////////////////////////////////////////
//
// googleChromeUtil class is provided to export google
// googleChromeUtil bookmarks to IE favorites
//
// by 365days.enjoy#gmail.com 
//   & alan.ncu#gmail.com
//                                                 2010.4.14
// Modified by axus.web#gmail.com                  2010.10.2

#include <cstdio>
#include <limits.h>

//Fixes for MinGW
#ifdef MINGW
#include "fixmingw.h"
#endif

#include "googleChromeUtil.h"

using std::wcout;
using std::wcerr;
using std::endl;
using std::flush;

bool mk_url_obj(std::string id, std::string name, std::string url, Json::Value& obj)
{
	char format[]=" {\
		\"date_added\": \"0\",\
		\"id\": \"%s\",\
		\"name\": \"%s\",\
		\"type\": \"url\",\
		\"url\": \"%s\"}";
	char sto[1000];
	sprintf_s(sto, format, id.c_str(), name.c_str(), url.c_str());
	Json::Reader reader;
	obj.clear();
	return reader.parse(sto,obj);	
}

bool mk_folder_obj(std::string id, std::string name, Json::Value& obj)
{
     char format[]=" { \"children\": [ ],\
				   \"date_added\": \"12915246468212928\",\
								\"date_modified\": \"12915248079990011\",\
								\"id\": \"%s\",\
								\"name\": \"%s\",\
								\"type\": \"folder\" }";
	 char sto[1000];
	 sprintf_s(sto, format, id.c_str(), name.c_str());

	 Json::Reader reader;
	 obj.clear();
	 return reader.parse(sto,obj);
}

#include <shlobj.h>
#include <shlwapi.h>

//pragma not used by MinGW!  Use -lshlwapi
#ifndef MINGW
#pragma comment(lib, "shlwapi.lib")
#endif

bool googleChromeUtil::is_installed_chrome(wchar_t * path)
{
	wchar_t path_out[PATH_MAX]={0};
	SHGetSpecialFolderPathW(NULL, path_out, CSIDL_APPDATA, FALSE);
	wchar_t *pt=wcsrchr(path_out, L'\\');
	if (pt)
	{
	   *pt=L'\0';
	}

	lstrcatW(path_out, L"\\Local\\Google\\Chrome\\User Data\\Default\\Bookmarks");
	BOOL ok= PathFileExistsW(path_out);	
	if (ok)
	{
		if (lstrlenW(path_out)>8)
		{
			lstrcpynW(path, path_out, PATH_MAX - 1);
			return true;
		}
		return true;
	}
	return false;
}

bool googleChromeUtil::is_chrome_running()
{
	HWND handle=::FindWindowW(L"Chrome_WindowImpl_0", L"Google - Google Chrome");
    
	if (handle)
		return true;


	return false;
}

bool googleChromeUtil::close_chrome()
{
	HWND handle=::FindWindowW(L"Chrome_WindowImpl_0", L"Google - Google Chrome");
	if (handle)
	{
		::SendMessage(handle, WM_CLOSE, 0, 0);
	}

    handle=::FindWindowW(L"Chrome_WindowImpl_0", L"Google - Google Chrome");
	if (handle)
	{
		return false;
	}
	return true;
}

#include <fstream>
using namespace std;

bool googleChromeUtil::load(const wchar_t *file_path)
{	
	wchar_t final[PATH_MAX]={0};
	char target[600]={0};
	if (!file_path)
	{
		wchar_t get_path[260]={0};
		bool ok=is_installed_chrome(get_path);
		if (ok)
		{   
            lstrcpynW(final, get_path, PATH_MAX - 1);
		}
	}
	else
		lstrcpynW(final, file_path, PATH_MAX - 1);

	if (lstrlenW(final)>8)
	{
		convert_wchar_2_utf8(final, target);

		chrome_.clear();
        
		ifstream infile(target);

		Json::Reader reader;
		bool ok= reader.parse(infile, chrome_);
		infile.close();
		return ok;
	}

	return false;
}

// the user must make sure that the url_obj is a url.
void googleChromeUtil::export_url(Json::Value& url_obj, const wchar_t* path)
{

        //Ignore empty URLs
        if (url_obj.empty() || url_obj["type"].asString() !=std::string("url"))
                return ;

        //Replace invalid Windows filename characters
	std::string name=url_obj["name"].asString();
	size_t charpos;
	while ( (charpos = name.find_first_of("/\\?*:<>|\"")) != string::npos) {
                name[charpos] = '-';
        }

        //Convert URL name to wide character
	wchar_t *wname = new wchar_t[name.length() + 1]; wname[0] = '\0';
	convert_utf8_2_wchar(name.c_str(), wname);
	
	//Create wchar array for complete file path + name
	wchar_t *file = new wchar_t[PATH_MAX + name.length() + 8]; file[0] = '\0';
	
	//Concatenate PATH + \ + NAME + .url
	lstrcpynW(file, path, wcsnlen(path, PATH_MAX - 1) + 1);
	lstrcatW(file, L"\\");
	lstrcatW(file, wname);
	lstrcatW(file, L".url");

	
	//Create wchar array for URL
	std::string http=url_obj["url"].asString();
	wchar_t *url = new wchar_t[http.length() + 1]; url[0] = '\0';
	convert_utf8_2_wchar(http.c_str(), url);


        //Write .ini style URL file
        WritePrivateProfileStringW(L"InternetShortcut", L"URL", url , file);    


   
        //Clean up those arrays we created!
        delete file; delete wname; delete url;
}

// the user must make sure that the folder_obj is a folder.
void googleChromeUtil::export_folder(Json::Value& folder_obj, const wchar_t* path)
{

        //Ignore empty folder names and non-folders
	if (folder_obj.empty() || folder_obj["type"].asString() != std::string("folder"))
		return;

        //The Favorites folder we create can't be longer than PATH_MAX characters
        size_t maxFolderLength = PATH_MAX - 1 - wcsnlen(path, PATH_MAX - 3);

        //Get the bookmark folder name, convert to wide characters
	std::string folder=folder_obj["name"].asString().substr(0,maxFolderLength);
	wchar_t *folder_name = new wchar_t[maxFolderLength + 1]; folder_name[0] = '\0';
	convert_utf8_2_wchar(folder.c_str(), folder_name);

	// Create new_path string (including NULL terminator!)
	wchar_t new_path[PATH_MAX]={0};
	lstrcpynW(new_path, path, wcsnlen(path, PATH_MAX - 3) + 1);
	lstrcatW(new_path, L"\\");
	lstrcatW(new_path, folder_name);

	// create a new folder.
	CreateDirectoryW(new_path, NULL);

	// iterate through to export.
	Json::Value arr=folder_obj["children"];
	int count=arr.size();
	for (int i=0; i<count; ++i)
	{

		Json::Value new_val=arr[i];
		if (new_val["type"].asString()==std::string("url"))
		{
			export_url(new_val, new_path);
		}
		else if(new_val["type"].asString()==std::string("folder"))
		{
			export_folder(new_val, new_path);
		}
	}
}

//Export the Chrome Bookmarks to a folder
// folder_path is full Windows pathname to Favorites folder
bool googleChromeUtil::export_2_folder(const wchar_t* folder_path)
{
    if (!folder_path || chrome_.empty())
       return false;
   
	Json::Value roots=chrome_["roots"];
	
	//Export "Bookmarks bar" to a subfolder
	export_folder(roots["bookmark_bar"], folder_path);
	
	//Export "Other Bookmarks" to Favorites
        // iterate through to export.
	Json::Value arr=roots["other"]["children"];
		
	size_t index;
        size_t num_children=arr.size();
	for (index=0; index < num_children; index++)
	{
		Json::Value new_val=arr[index];
                //Export URL if type="url"
		if (new_val["type"].asString()==std::string("url"))
		{
			export_url(new_val, folder_path);
		}
		//Export folder if type="folder"
		else if(new_val["type"].asString()==std::string("folder"))
		{
			export_folder(new_val, folder_path);
		}
		//Else, ignore it
	}

        //This exports "other" to a separate folder, instead of straight to Favorites	
	//export_folder(roots["other"], folder_path);

	return true;
}
