#ifndef __STDX_STRING_H
#define __STDX_STRING_H


// Posix header files

// C 89 header files

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

// C++ 98 header files
#include <string>
#include <sstream>


namespace stdx {


template <typename _Tp>
inline _Tp
from_string(const std::string& str) {
    _Tp x;
    std::istringstream iss(str);
    iss >> x;
    return x;
}

template <typename _Tp>
inline _Tp&
from_string(const std::string& str, _Tp& x) {
    std::istringstream iss(str);
    iss >> x;
    return x;
}

template <typename _Tp>
inline std::string
to_string(const _Tp& x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

inline std::string
stdx_strerror(const std::string& strprefix = "") {
    char strerr[128] = { 0 };
    strerror_r(errno, strerr, sizeof(strerr));
    return strprefix + strerr;
}

const std::string json_number_chars = "0123456789.+-eE";
const std::string json_hex_chars = "0123456789abcdef";

inline int
stdx_escape_string(std::string &buf, const std::string& orig)
{
    //*/
    for (std::string::size_type i = 0; i < orig.size(); ++i)
    {
        std::string::value_type c = orig[i];

        switch (c)
        {
        case '\0':
            break;
        case '\b':
        case '\n':
        case '\r':
        case '\t':
        case '"':
        case '\\':
        case '/':
            if (c == '\b')
                buf.append("\\b", 2);
            else if (c == '\n')
                buf.append("\\n", 2);
            else if (c == '\r')
                buf.append("\\r", 2);
            else if (c == '\t')
                buf.append("\\t", 2);
            else if (c == '"')
                buf.append("\\\"", 2);
            else if (c == '\\')
                buf.append("\\\\", 2);
            else if (c == '/')
                buf.append("\\/", 2);
            break;
        default:
            if (c < ' ')
            {
                buf += std::string("\\u00")
                    + json_hex_chars[c >> 6] + json_hex_chars[c & 0xf];
            }
            else
            {
                buf += c;
            }
        }
    }
    return 0;

    /*/

    int pos = 0, start_offset = 0;
    do
    {
        std::string::value_type c = orig[pos];
        switch (c)
        {
        case '\0':
            break;
        case '\b':
        case '\n':
        case '\r':
        case '\t':
        case '"':
        case '\\':
        case '/':
            if (pos - start_offset > 0)
                buf.append(orig.substr(start_offset, pos - start_offset));
            if (c == '\b')
                buf.append("\\b", 2);
            else if (c == '\n')
                buf.append("\\n", 2);
            else if (c == '\r')
                buf.append("\\r", 2);
            else if (c == '\t')
                buf.append("\\t", 2);
            else if (c == '"')
                buf.append("\\\"", 2);
            else if (c == '\\')
                buf.append("\\\\", 2);
            else if (c == '/')
                buf.append("\\/", 2);
            start_offset = ++pos;
            break;
        default:
            if (c < ' ')
            {
                if (pos - start_offset > 0)
                    buf.append(orig.substr(start_offset, pos - start_offset));
                buf += std::string("\\u00")
                    + json_hex_chars[c >> 6] + json_hex_chars[c & 0xf];
                start_offset = ++pos;
            }
            else
                pos++;
        }
    } while(c);
    if (pos - start_offset > 0)
        buf.append(orig.substr(start_offset, pos - start_offset));
    return 0;
    */
}



/**
 ** ulong_to_hex : convert ulong to a 16-bit hex-str
 ** for performance
 */
inline 
void ulong_to_hex(const uint64_t val, char* str)
{
    // make sure the str buff is larger than 17
    const char* table = "0123456789abcdef";

    uint64_t mask = 0xF;
    uint64_t idx;
    for(int i=0; i<16; ++i)
    {
       idx = (val & mask) >> 4*i;
       str[15-i] = table[idx];
       mask = mask << 4;
    }

    str[16] = 0;
}

/**
 ** hex_to_ulong : convert a 16-bit hex-str to ulong
 ** for performance
 */
inline 
uint64_t hex_to_ulong(const char* str)
{
    uint64_t var = 0;

    uint64_t temp;
    for(int i=0; i<16; ++i)
    {
        char c = str[i];
        if( c >= 'a')
            temp = c- 'a' + 10;
        else
            temp = c- '0';

        temp = temp << 4*(15-i);
        var += temp;
    }

    return var;
}


inline 
std::string to_hex_string(unsigned char *inputData, size_t size)  
{  
    char temp[4];  
    std::string szHexOutput;  

    for (size_t i = 0 ; i < size ; i++)  
    {  
        sprintf(temp, "%02X ", inputData[i]);  
        szHexOutput += std::string(temp,3);  
    }  
    return szHexOutput;  
}  

inline 
std::string to_hex_string(const std::string &inputStr)  
{  
    char temp[4];  
    std::string szHexOutput;  

    for (size_t i = 0 ; i < inputStr.length() ; i++)  
    {  
        sprintf(temp, "%02X ", (unsigned char)inputStr[i]);  
        szHexOutput += std::string(temp,3);  
    }  
    return szHexOutput;  
}

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
#ifndef HAVE_STRLCPY
inline size_t
strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}
#else
inline size_t
strlcpy(char *dst, const char *src, size_t siz)
{
    ::strlcpy(dst, src, siz);
}
#endif 

} // namespace stdx


#endif // __STDX_STRING_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
