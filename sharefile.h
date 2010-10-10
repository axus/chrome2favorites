
#ifndef SHARE_FILE__H_
#define SHARE_FILE__H_
#include <Windows.h>

bool convert_utf8_2_wchar(const char* utf8, wchar_t * wchar_out);


size_t convert_wchar_2_utf8(const wchar_t* wchar_in, char * utf8);

bool get_ie_path(wchar_t* ie);

#endif