//HACKS to make MinGW compiler support some functions used by other compilers

//"Safe" sprintf
#ifndef sprintf_s
#define sprintf_s sprintf
#endif

//Count length of wchar_t array by finding null terminator
#ifndef wcsnlen
#include <algorithm>
std::size_t wcsnlen(const wchar_t *str, size_t max) {
        wchar_t null_value = '\0';
        size_t result = 0;

        //Check every value in the array for null_value
        const wchar_t *null_index = std::find(str, str + max, null_value);
        
        //Return number of non-null characters
        result = null_index - str;
        return result;
}
#endif