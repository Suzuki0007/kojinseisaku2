#include "pch.h"
#include "utf8sjis.h"

static int( *s_pfunc_SJIStoUTF8 )( const char* pszSjis, char8_t* pszOutUtf8 ) = nullptr;

int Utf8Sjis::UTF8_OctBytes(char8_t c)
{
	if((c & 0x80) == 0x00) { return 1; }
	if((c & 0xe0) == 0xc0) { return 2; }
	if((c & 0xf0) == 0xe0) { return 3; }
	if((c & 0xf8) == 0xf0) { return 4; }
	if((c & 0xfc) == 0xf8) { return 5; }
	if((c & 0xfe) == 0xfc) { return 6; }
	return 0;
}
