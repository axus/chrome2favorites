//////////////////////////////////////////////////////////////////////////
//
// Export google chrome bookmarks to IE favorites
//
// by 365days.enjoy#gmail.com 
//   & alan.ncu#gmail.com          2010.4.14
// Modified by axus.buy#gmail.com  2010.10.2

#include <iostream>
#include "json/json.h"
#include <fstream>
#include "googleChromeUtil.h"
#include "sharefile.h"
#include <limits.h>

using std::wcout;
using std::endl;

int main()
{
	wchar_t path[PATH_MAX]={0};
	wchar_t bkmk[PATH_MAX]={0};
	
        //Check IE favorites path
	if (!get_ie_path(path)) {
		wcout << L"Could not get Internet Explorer favorites folder" <<endl;
                return 1;
        }
	wcout << L"Internet Explorer favorites folder is " <<path <<endl;

        //Check Chrome bookmarks path
	if(!googleChromeUtil::is_installed_chrome(bkmk))
	{
                // You could substitute a filename here if you wanted to update this ;p
                wcout<<L"Chrome bookmarks file not found" << endl;
                return 1;
	}
	wcout<<L"Chrome bookmarks file is "<<bkmk<<endl;

	//Uncomment this to export to a subdirectory of Favorites
	//lstrcatW(path, L"\\Google chrome"); 
	//CreateDirectoryW(path, NULL);

        //Perform the import and export
	googleChromeUtil obj;
	obj.load(bkmk);              
	obj.export_2_folder(path);

        //Complete!
        wcout<<L"Complete!"<<endl;
        return 0;
}
