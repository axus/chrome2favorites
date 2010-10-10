//Windows-only functions for converting UTF8 to WCHAR and back

#include "sharefile.h"
#include "tchar.h"

using std::string;
using std::wstring;

bool convert_utf8_2_wchar(const char* utf8, wchar_t * wchar_out)
{
	if (!utf8 || !wchar_out)
	   return false;

	int len=(int)strlen(utf8)+1;

	MultiByteToWideChar(CP_UTF8, 0, utf8, len, wchar_out, len);
	return true;
}


size_t convert_wchar_2_utf8(const wchar_t* wchar_in, char * utf8)
{
	if (!utf8 || !wchar_in)
		return 0;

	int len=lstrlenW(wchar_in)+1;

	int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, wchar_in , len , 0, 0, 0, 0);

	utf8[0]=0;

	WideCharToMultiByte(CP_UTF8, 0, wchar_in, len , utf8, iRequiredSize, 0, 0);
	return iRequiredSize;
}

//Convert string to wstring
wstring string_2_wstring(const string& input)
{
        //Create wstring same length as string
        wstring output(input.length(), L' ');
        
        //Use STL copy algorithm to copy
        std::copy(input.begin(), input.end(), output.begin());
        
        return output;
}

//Convert wstring to string
string wstring_2_string(const wstring& input)
{
        //Create wstring same length as string
        string output(input.length(), ' ');
        
        //Use STL copy algorithm to copy
        std::copy(input.begin(), input.end(), output.begin());
        
        return output;
}


#include <shlobj.h>

bool get_ie_path(wchar_t* ie)
{
    return SHGetSpecialFolderPathW(NULL, ie, CSIDL_FAVORITES, FALSE);
}
