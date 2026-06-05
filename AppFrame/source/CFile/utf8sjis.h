#pragma once

#ifdef WIN32
#include <windows.h>
#endif
#include <string>
#include <string_view>

class Utf8Sjis
{
public:
	bool SJIS_IsKanji(unsigned char cData);
	int UTF8_OctBytes(char8_t c);

	std::string ConvUTF8toSJIS(std::u8string_view strUtf8);
	std::u8string ConvSJIStoUTF8(std::string_view strSjis);

	int ConvUTF8toSJIS(const char8_t* pszUtf8, char* pszOutSjis, int nOutSjisSize);
	int ConvSJIStoUTF8(const char* pszSjis, char8_t* pszOutUtf8);

	char32_t ConvUTF8toUTF32Single(const char8_t* input);

private:
	static int( *s_pfunc_SJIStoUTF8 )( char* pSJIS_Single, int nInSize, char* pUTF8_Buffer );
	static int()

};

