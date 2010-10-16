
#ifndef SHARE_FILE__H_
#define SHARE_FILE__H_
#include <Windows.h>
#include <string>

bool convert_utf8_2_wchar(const char* utf8, wchar_t * wchar_out);
size_t convert_wchar_2_utf8(const wchar_t* wchar_in, char * utf8);

std::string wstring_2_string(const std::wstring& input);
std::wstring string_2_wstring(const std::string& input);

bool get_ie_path(wchar_t* ie);

#endif