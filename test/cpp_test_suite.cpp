///////////////////////////////////////////////////////////////////////////////
// \author (c) C VERSION -- Marco Paland (info@paland.com)
//             2014-2018, PALANDesign Hannover, Germany
//         (c) CPP HEADER ONLY VERSION -- Dusan B. Jovanovic ( dbj@dbj.org )
//             2018, dbj.systems, London, UK 
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////

// use the 'catch' test framework
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define PRINTF_USER_DEFINED_PUTCHAR
// inside the namespace mpaland_dbjdbj
#include "../printf++.h"

#include <string.h>

// dummy putchar
static char   printf_buffer[100]{};
static std::size_t printf_idx{ 0U };

namespace mpaland_dbjdbj {
	inline void _putchar(char character)
	{
		printf_buffer[printf_idx++] = character;
	}
}

namespace {

void _out_fct(char character, void* arg)
{
	(void)arg;
	printf_buffer[printf_idx++] = character;
}

TEST_CASE("printf", "[]") {
	printf_idx = 0U;
	memset(printf_buffer, 0xCC, 100U);
	REQUIRE(mpaland_dbjdbj::printf("% d", 4232) == 5);
	REQUIRE(printf_buffer[5] == (char)0xCC);
	printf_buffer[5] = 0;
	REQUIRE(!strcmp(printf_buffer, " 4232"));
}


TEST_CASE("fctprintf", "[]") {
	printf_idx = 0U;
	memset(printf_buffer, 0xCC, 100U);
	mpaland_dbjdbj::fctprintf(&_out_fct, nullptr, "This is a test of %X", 0x12EFU);
	REQUIRE(!strcmp(printf_buffer, "This is a test of 12EF"));
}


TEST_CASE("snprintf", "[]") {
	char buffer[100];

	mpaland_dbjdbj::snprintf(buffer, 100U, "%d", -1000);
	REQUIRE(!strcmp(buffer, "-1000"));

	mpaland_dbjdbj::snprintf(buffer, 3U, "%d", -1000);
	REQUIRE(!strcmp(buffer, "-1"));
}


static void vsnprintf_builder_1(char* buffer, ...)
{
	va_list args;
	va_start(args, buffer);
	mpaland_dbjdbj::vsnprintf(buffer, 100U, "%d", args);
	va_end(args);
}

static void vsnprintf_builder_3(char* buffer, ...)
{
	va_list args;
	va_start(args, buffer);
	mpaland_dbjdbj::vsnprintf(buffer, 100U, "%d %d %s", args);
	va_end(args);
}

TEST_CASE("vsnprintf", "[]") {
	char buffer[100];

	vsnprintf_builder_1(buffer, -1);
	REQUIRE(!strcmp(buffer, "-1"));

	vsnprintf_builder_3(buffer, 3, -1000, "test");
	REQUIRE(!strcmp(buffer, "3 -1000 test"));
}


TEST_CASE("space flag", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "% d", 42);
	REQUIRE(!strcmp(buffer, " 42"));

	mpaland_dbjdbj::sprintf(buffer, "% d", -42);
	REQUIRE(!strcmp(buffer, "-42"));

	mpaland_dbjdbj::sprintf(buffer, "% 5d", 42);
	REQUIRE(!strcmp(buffer, "   42"));

	mpaland_dbjdbj::sprintf(buffer, "% 5d", -42);
	REQUIRE(!strcmp(buffer, "  -42"));

	mpaland_dbjdbj::sprintf(buffer, "% 15d", 42);
	REQUIRE(!strcmp(buffer, "             42"));

	mpaland_dbjdbj::sprintf(buffer, "% 15d", -42);
	REQUIRE(!strcmp(buffer, "            -42"));

	mpaland_dbjdbj::sprintf(buffer, "% 15d", -42);
	REQUIRE(!strcmp(buffer, "            -42"));

	mpaland_dbjdbj::sprintf(buffer, "% 15.3f", -42.987);
	REQUIRE(!strcmp(buffer, "        -42.987"));

	mpaland_dbjdbj::sprintf(buffer, "% 15.3f", 42.987);
	REQUIRE(!strcmp(buffer, "         42.987"));

	mpaland_dbjdbj::sprintf(buffer, "% s", "Hello testing");
	REQUIRE(!strcmp(buffer, "Hello testing"));

	mpaland_dbjdbj::sprintf(buffer, "% d", 1024);
	REQUIRE(!strcmp(buffer, " 1024"));

	mpaland_dbjdbj::sprintf(buffer, "% d", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "% i", 1024);
	REQUIRE(!strcmp(buffer, " 1024"));

	mpaland_dbjdbj::sprintf(buffer, "% i", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "% u", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "% u", 4294966272U);
	REQUIRE(!strcmp(buffer, "4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "% o", 511);
	REQUIRE(!strcmp(buffer, "777"));

	mpaland_dbjdbj::sprintf(buffer, "% o", 4294966785U);
	REQUIRE(!strcmp(buffer, "37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "% x", 305441741);
	REQUIRE(!strcmp(buffer, "1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "% x", 3989525555U);
	REQUIRE(!strcmp(buffer, "edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "% X", 305441741);
	REQUIRE(!strcmp(buffer, "1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "% X", 3989525555U);
	REQUIRE(!strcmp(buffer, "EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "% c", 'x');
	REQUIRE(!strcmp(buffer, "x"));
}


TEST_CASE("+ flag", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%+d", 42);
	REQUIRE(!strcmp(buffer, "+42"));

	mpaland_dbjdbj::sprintf(buffer, "%+d", -42);
	REQUIRE(!strcmp(buffer, "-42"));

	mpaland_dbjdbj::sprintf(buffer, "%+5d", 42);
	REQUIRE(!strcmp(buffer, "  +42"));

	mpaland_dbjdbj::sprintf(buffer, "%+5d", -42);
	REQUIRE(!strcmp(buffer, "  -42"));

	mpaland_dbjdbj::sprintf(buffer, "%+15d", 42);
	REQUIRE(!strcmp(buffer, "            +42"));

	mpaland_dbjdbj::sprintf(buffer, "%+15d", -42);
	REQUIRE(!strcmp(buffer, "            -42"));

	mpaland_dbjdbj::sprintf(buffer, "%+s", "Hello testing");
	REQUIRE(!strcmp(buffer, "Hello testing"));

	mpaland_dbjdbj::sprintf(buffer, "%+d", 1024);
	REQUIRE(!strcmp(buffer, "+1024"));

	mpaland_dbjdbj::sprintf(buffer, "%+d", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "%+i", 1024);
	REQUIRE(!strcmp(buffer, "+1024"));

	mpaland_dbjdbj::sprintf(buffer, "%+i", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "%+u", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%+u", 4294966272U);
	REQUIRE(!strcmp(buffer, "4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%+o", 511);
	REQUIRE(!strcmp(buffer, "777"));

	mpaland_dbjdbj::sprintf(buffer, "%+o", 4294966785U);
	REQUIRE(!strcmp(buffer, "37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%+x", 305441741);
	REQUIRE(!strcmp(buffer, "1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%+x", 3989525555U);
	REQUIRE(!strcmp(buffer, "edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%+X", 305441741);
	REQUIRE(!strcmp(buffer, "1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%+X", 3989525555U);
	REQUIRE(!strcmp(buffer, "EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "%+c", 'x');
	REQUIRE(!strcmp(buffer, "x"));
}


TEST_CASE("0 flag", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%0d", 42);
	REQUIRE(!strcmp(buffer, "42"));

	mpaland_dbjdbj::sprintf(buffer, "%0ld", 42L);
	REQUIRE(!strcmp(buffer, "42"));

	mpaland_dbjdbj::sprintf(buffer, "%0d", -42);
	REQUIRE(!strcmp(buffer, "-42"));

	mpaland_dbjdbj::sprintf(buffer, "%05d", 42);
	REQUIRE(!strcmp(buffer, "00042"));

	mpaland_dbjdbj::sprintf(buffer, "%05d", -42);
	REQUIRE(!strcmp(buffer, "-0042"));

	mpaland_dbjdbj::sprintf(buffer, "%015d", 42);
	REQUIRE(!strcmp(buffer, "000000000000042"));

	mpaland_dbjdbj::sprintf(buffer, "%015d", -42);
	REQUIRE(!strcmp(buffer, "-00000000000042"));

	mpaland_dbjdbj::sprintf(buffer, "%015.2f", 42.1234);
	REQUIRE(!strcmp(buffer, "000000000042.12"));

	mpaland_dbjdbj::sprintf(buffer, "%015.3f", 42.9876);
	REQUIRE(!strcmp(buffer, "00000000042.988"));

	mpaland_dbjdbj::sprintf(buffer, "%015.5f", -42.9876);
	REQUIRE(!strcmp(buffer, "-00000042.98760"));
}


TEST_CASE("- flag", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%-d", 42);
	REQUIRE(!strcmp(buffer, "42"));

	mpaland_dbjdbj::sprintf(buffer, "%-d", -42);
	REQUIRE(!strcmp(buffer, "-42"));

	mpaland_dbjdbj::sprintf(buffer, "%-5d", 42);
	REQUIRE(!strcmp(buffer, "42   "));

	mpaland_dbjdbj::sprintf(buffer, "%-5d", -42);
	REQUIRE(!strcmp(buffer, "-42  "));

	mpaland_dbjdbj::sprintf(buffer, "%-15d", 42);
	REQUIRE(!strcmp(buffer, "42             "));

	mpaland_dbjdbj::sprintf(buffer, "%-15d", -42);
	REQUIRE(!strcmp(buffer, "-42            "));

	mpaland_dbjdbj::sprintf(buffer, "%-0d", 42);
	REQUIRE(!strcmp(buffer, "42"));

	mpaland_dbjdbj::sprintf(buffer, "%-0d", -42);
	REQUIRE(!strcmp(buffer, "-42"));

	mpaland_dbjdbj::sprintf(buffer, "%-05d", 42);
	REQUIRE(!strcmp(buffer, "42   "));

	mpaland_dbjdbj::sprintf(buffer, "%-05d", -42);
	REQUIRE(!strcmp(buffer, "-42  "));

	mpaland_dbjdbj::sprintf(buffer, "%-015d", 42);
	REQUIRE(!strcmp(buffer, "42             "));

	mpaland_dbjdbj::sprintf(buffer, "%-015d", -42);
	REQUIRE(!strcmp(buffer, "-42            "));

	mpaland_dbjdbj::sprintf(buffer, "%0-d", 42);
	REQUIRE(!strcmp(buffer, "42"));

	mpaland_dbjdbj::sprintf(buffer, "%0-d", -42);
	REQUIRE(!strcmp(buffer, "-42"));

	mpaland_dbjdbj::sprintf(buffer, "%0-5d", 42);
	REQUIRE(!strcmp(buffer, "42   "));

	mpaland_dbjdbj::sprintf(buffer, "%0-5d", -42);
	REQUIRE(!strcmp(buffer, "-42  "));

	mpaland_dbjdbj::sprintf(buffer, "%0-15d", 42);
	REQUIRE(!strcmp(buffer, "42             "));

	mpaland_dbjdbj::sprintf(buffer, "%0-15d", -42);
	REQUIRE(!strcmp(buffer, "-42            "));
}


TEST_CASE("specifier", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "Hello testing");
	REQUIRE(!strcmp(buffer, "Hello testing"));

	mpaland_dbjdbj::sprintf(buffer, "%s", "Hello testing");
	REQUIRE(!strcmp(buffer, "Hello testing"));

	mpaland_dbjdbj::sprintf(buffer, "%d", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%d", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "%i", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%i", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "%u", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%u", 4294966272U);
	REQUIRE(!strcmp(buffer, "4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%o", 511);
	REQUIRE(!strcmp(buffer, "777"));

	mpaland_dbjdbj::sprintf(buffer, "%o", 4294966785U);
	REQUIRE(!strcmp(buffer, "37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%x", 305441741);
	REQUIRE(!strcmp(buffer, "1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%x", 3989525555U);
	REQUIRE(!strcmp(buffer, "edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%X", 305441741);
	REQUIRE(!strcmp(buffer, "1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%X", 3989525555U);
	REQUIRE(!strcmp(buffer, "EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "%%");
	REQUIRE(!strcmp(buffer, "%"));
}


TEST_CASE("width", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%1s", "Hello testing");
	REQUIRE(!strcmp(buffer, "Hello testing"));

	mpaland_dbjdbj::sprintf(buffer, "%1d", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%1d", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "%1i", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%1i", -1024);
	REQUIRE(!strcmp(buffer, "-1024"));

	mpaland_dbjdbj::sprintf(buffer, "%1u", 1024);
	REQUIRE(!strcmp(buffer, "1024"));

	mpaland_dbjdbj::sprintf(buffer, "%1u", 4294966272U);
	REQUIRE(!strcmp(buffer, "4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%1o", 511);
	REQUIRE(!strcmp(buffer, "777"));

	mpaland_dbjdbj::sprintf(buffer, "%1o", 4294966785U);
	REQUIRE(!strcmp(buffer, "37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%1x", 305441741);
	REQUIRE(!strcmp(buffer, "1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%1x", 3989525555U);
	REQUIRE(!strcmp(buffer, "edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%1X", 305441741);
	REQUIRE(!strcmp(buffer, "1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%1X", 3989525555U);
	REQUIRE(!strcmp(buffer, "EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "%1c", 'x');
	REQUIRE(!strcmp(buffer, "x"));
}


TEST_CASE("width 20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%20s", "Hello");
	REQUIRE(!strcmp(buffer, "               Hello"));

	mpaland_dbjdbj::sprintf(buffer, "%20d", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20d", -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20i", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20i", -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20u", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20u", 4294966272U);
	REQUIRE(!strcmp(buffer, "          4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%20o", 511);
	REQUIRE(!strcmp(buffer, "                 777"));

	mpaland_dbjdbj::sprintf(buffer, "%20o", 4294966785U);
	REQUIRE(!strcmp(buffer, "         37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%20x", 305441741);
	REQUIRE(!strcmp(buffer, "            1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%20x", 3989525555U);
	REQUIRE(!strcmp(buffer, "            edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%20X", 305441741);
	REQUIRE(!strcmp(buffer, "            1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%20X", 3989525555U);
	REQUIRE(!strcmp(buffer, "            EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "%20c", 'x');
	REQUIRE(!strcmp(buffer, "                   x"));
}


TEST_CASE("width *20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%*s", 20, "Hello");
	REQUIRE(!strcmp(buffer, "               Hello"));

	mpaland_dbjdbj::sprintf(buffer, "%*d", 20, 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%*d", 20, -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%*i", 20, 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%*i", 20, -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%*u", 20, 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%*u", 20, 4294966272U);
	REQUIRE(!strcmp(buffer, "          4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%*o", 20, 511);
	REQUIRE(!strcmp(buffer, "                 777"));

	mpaland_dbjdbj::sprintf(buffer, "%*o", 20, 4294966785U);
	REQUIRE(!strcmp(buffer, "         37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%*x", 20, 305441741);
	REQUIRE(!strcmp(buffer, "            1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%*x", 20, 3989525555U);
	REQUIRE(!strcmp(buffer, "            edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%*X", 20, 305441741);
	REQUIRE(!strcmp(buffer, "            1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%*X", 20, 3989525555U);
	REQUIRE(!strcmp(buffer, "            EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "%*c", 20, 'x');
	REQUIRE(!strcmp(buffer, "                   x"));
}


TEST_CASE("width -20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%-20s", "Hello");
	REQUIRE(!strcmp(buffer, "Hello               "));

	mpaland_dbjdbj::sprintf(buffer, "%-20d", 1024);
	REQUIRE(!strcmp(buffer, "1024                "));

	mpaland_dbjdbj::sprintf(buffer, "%-20d", -1024);
	REQUIRE(!strcmp(buffer, "-1024               "));

	mpaland_dbjdbj::sprintf(buffer, "%-20i", 1024);
	REQUIRE(!strcmp(buffer, "1024                "));

	mpaland_dbjdbj::sprintf(buffer, "%-20i", -1024);
	REQUIRE(!strcmp(buffer, "-1024               "));

	mpaland_dbjdbj::sprintf(buffer, "%-20u", 1024);
	REQUIRE(!strcmp(buffer, "1024                "));

	mpaland_dbjdbj::sprintf(buffer, "%-20.4f", 1024.1234);
	REQUIRE(!strcmp(buffer, "1024.1234           "));

	mpaland_dbjdbj::sprintf(buffer, "%-20u", 4294966272U);
	REQUIRE(!strcmp(buffer, "4294966272          "));

	mpaland_dbjdbj::sprintf(buffer, "%-20o", 511);
	REQUIRE(!strcmp(buffer, "777                 "));

	mpaland_dbjdbj::sprintf(buffer, "%-20o", 4294966785U);
	REQUIRE(!strcmp(buffer, "37777777001         "));

	mpaland_dbjdbj::sprintf(buffer, "%-20x", 305441741);
	REQUIRE(!strcmp(buffer, "1234abcd            "));

	mpaland_dbjdbj::sprintf(buffer, "%-20x", 3989525555U);
	REQUIRE(!strcmp(buffer, "edcb5433            "));

	mpaland_dbjdbj::sprintf(buffer, "%-20X", 305441741);
	REQUIRE(!strcmp(buffer, "1234ABCD            "));

	mpaland_dbjdbj::sprintf(buffer, "%-20X", 3989525555U);
	REQUIRE(!strcmp(buffer, "EDCB5433            "));

	mpaland_dbjdbj::sprintf(buffer, "%-20c", 'x');
	REQUIRE(!strcmp(buffer, "x                   "));

	mpaland_dbjdbj::sprintf(buffer, "|%5d| |%-2d| |%5d|", 9, 9, 9);
	REQUIRE(!strcmp(buffer, "|    9| |9 | |    9|"));

	mpaland_dbjdbj::sprintf(buffer, "|%5d| |%-2d| |%5d|", 10, 10, 10);
	REQUIRE(!strcmp(buffer, "|   10| |10| |   10|"));

	mpaland_dbjdbj::sprintf(buffer, "|%5d| |%-12d| |%5d|", 9, 9, 9);
	REQUIRE(!strcmp(buffer, "|    9| |9           | |    9|"));

	mpaland_dbjdbj::sprintf(buffer, "|%5d| |%-12d| |%5d|", 10, 10, 10);
	REQUIRE(!strcmp(buffer, "|   10| |10          | |   10|"));
}


TEST_CASE("width 0-20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%0-20s", "Hello");
	REQUIRE(!strcmp(buffer, "Hello               "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20d", 1024);
	REQUIRE(!strcmp(buffer, "1024                "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20d", -1024);
	REQUIRE(!strcmp(buffer, "-1024               "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20i", 1024);
	REQUIRE(!strcmp(buffer, "1024                "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20i", -1024);
	REQUIRE(!strcmp(buffer, "-1024               "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20u", 1024);
	REQUIRE(!strcmp(buffer, "1024                "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20u", 4294966272U);
	REQUIRE(!strcmp(buffer, "4294966272          "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20o", 511);
	REQUIRE(!strcmp(buffer, "777                 "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20o", 4294966785U);
	REQUIRE(!strcmp(buffer, "37777777001         "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20x", 305441741);
	REQUIRE(!strcmp(buffer, "1234abcd            "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20x", 3989525555U);
	REQUIRE(!strcmp(buffer, "edcb5433            "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20X", 305441741);
	REQUIRE(!strcmp(buffer, "1234ABCD            "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20X", 3989525555U);
	REQUIRE(!strcmp(buffer, "EDCB5433            "));

	mpaland_dbjdbj::sprintf(buffer, "%0-20c", 'x');
	REQUIRE(!strcmp(buffer, "x                   "));
}


TEST_CASE("padding 20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%020d", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%020d", -1024);
	REQUIRE(!strcmp(buffer, "-0000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%020i", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%020i", -1024);
	REQUIRE(!strcmp(buffer, "-0000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%020u", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%020u", 4294966272U);
	REQUIRE(!strcmp(buffer, "00000000004294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%020o", 511);
	REQUIRE(!strcmp(buffer, "00000000000000000777"));

	mpaland_dbjdbj::sprintf(buffer, "%020o", 4294966785U);
	REQUIRE(!strcmp(buffer, "00000000037777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%020x", 305441741);
	REQUIRE(!strcmp(buffer, "0000000000001234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%020x", 3989525555U);
	REQUIRE(!strcmp(buffer, "000000000000edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%020X", 305441741);
	REQUIRE(!strcmp(buffer, "0000000000001234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%020X", 3989525555U);
	REQUIRE(!strcmp(buffer, "000000000000EDCB5433"));
}


TEST_CASE("padding .20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%.20d", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%.20d", -1024);
	REQUIRE(!strcmp(buffer, "-00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%.20i", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%.20i", -1024);
	REQUIRE(!strcmp(buffer, "-00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%.20u", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%.20u", 4294966272U);
	REQUIRE(!strcmp(buffer, "00000000004294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%.20o", 511);
	REQUIRE(!strcmp(buffer, "00000000000000000777"));

	mpaland_dbjdbj::sprintf(buffer, "%.20o", 4294966785U);
	REQUIRE(!strcmp(buffer, "00000000037777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%.20x", 305441741);
	REQUIRE(!strcmp(buffer, "0000000000001234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%.20x", 3989525555U);
	REQUIRE(!strcmp(buffer, "000000000000edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%.20X", 305441741);
	REQUIRE(!strcmp(buffer, "0000000000001234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%.20X", 3989525555U);
	REQUIRE(!strcmp(buffer, "000000000000EDCB5433"));
}


TEST_CASE("padding #020", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%#020d", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%#020d", -1024);
	REQUIRE(!strcmp(buffer, "-0000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%#020i", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%#020i", -1024);
	REQUIRE(!strcmp(buffer, "-0000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%#020u", 1024);
	REQUIRE(!strcmp(buffer, "00000000000000001024"));

	mpaland_dbjdbj::sprintf(buffer, "%#020u", 4294966272U);
	REQUIRE(!strcmp(buffer, "00000000004294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%#020o", 511);
	REQUIRE(!strcmp(buffer, "00000000000000000777"));

	mpaland_dbjdbj::sprintf(buffer, "%#020o", 4294966785U);
	REQUIRE(!strcmp(buffer, "00000000037777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%#020x", 305441741);
	REQUIRE(!strcmp(buffer, "0x00000000001234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%#020x", 3989525555U);
	REQUIRE(!strcmp(buffer, "0x0000000000edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%#020X", 305441741);
	REQUIRE(!strcmp(buffer, "0X00000000001234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%#020X", 3989525555U);
	REQUIRE(!strcmp(buffer, "0X0000000000EDCB5433"));
}


TEST_CASE("padding #20", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%#20d", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%#20d", -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%#20i", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%#20i", -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%#20u", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%#20u", 4294966272U);
	REQUIRE(!strcmp(buffer, "          4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%#20o", 511);
	REQUIRE(!strcmp(buffer, "                0777"));

	mpaland_dbjdbj::sprintf(buffer, "%#20o", 4294966785U);
	REQUIRE(!strcmp(buffer, "        037777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%#20x", 305441741);
	REQUIRE(!strcmp(buffer, "          0x1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%#20x", 3989525555U);
	REQUIRE(!strcmp(buffer, "          0xedcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%#20X", 305441741);
	REQUIRE(!strcmp(buffer, "          0X1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%#20X", 3989525555U);
	REQUIRE(!strcmp(buffer, "          0XEDCB5433"));
}


TEST_CASE("padding 20.5", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%20.5d", 1024);
	REQUIRE(!strcmp(buffer, "               01024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5d", -1024);
	REQUIRE(!strcmp(buffer, "              -01024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5i", 1024);
	REQUIRE(!strcmp(buffer, "               01024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5i", -1024);
	REQUIRE(!strcmp(buffer, "              -01024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5u", 1024);
	REQUIRE(!strcmp(buffer, "               01024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5u", 4294966272U);
	REQUIRE(!strcmp(buffer, "          4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5o", 511);
	REQUIRE(!strcmp(buffer, "               00777"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5o", 4294966785U);
	REQUIRE(!strcmp(buffer, "         37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5x", 305441741);
	REQUIRE(!strcmp(buffer, "            1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%20.10x", 3989525555U);
	REQUIRE(!strcmp(buffer, "          00edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%20.5X", 305441741);
	REQUIRE(!strcmp(buffer, "            1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%20.10X", 3989525555U);
	REQUIRE(!strcmp(buffer, "          00EDCB5433"));
}


TEST_CASE("length", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%.0s", "Hello testing");
	REQUIRE(!strcmp(buffer, ""));

	mpaland_dbjdbj::sprintf(buffer, "%20.0s", "Hello testing");
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%.s", "Hello testing");
	REQUIRE(!strcmp(buffer, ""));

	mpaland_dbjdbj::sprintf(buffer, "%20.s", "Hello testing");
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%20.0d", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.0d", -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.d", 0);
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%20.0i", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.i", -1024);
	REQUIRE(!strcmp(buffer, "               -1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.i", 0);
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%20.u", 1024);
	REQUIRE(!strcmp(buffer, "                1024"));

	mpaland_dbjdbj::sprintf(buffer, "%20.0u", 4294966272U);
	REQUIRE(!strcmp(buffer, "          4294966272"));

	mpaland_dbjdbj::sprintf(buffer, "%20.u", 0U);
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%20.o", 511);
	REQUIRE(!strcmp(buffer, "                 777"));

	mpaland_dbjdbj::sprintf(buffer, "%20.0o", 4294966785U);
	REQUIRE(!strcmp(buffer, "         37777777001"));

	mpaland_dbjdbj::sprintf(buffer, "%20.o", 0U);
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%20.x", 305441741);
	REQUIRE(!strcmp(buffer, "            1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%50.x", 305441741);
	REQUIRE(!strcmp(buffer, "                                          1234abcd"));

	mpaland_dbjdbj::sprintf(buffer, "%50.x%10.u", 305441741, 12345);
	REQUIRE(!strcmp(buffer, "                                          1234abcd     12345"));

	mpaland_dbjdbj::sprintf(buffer, "%20.0x", 3989525555U);
	REQUIRE(!strcmp(buffer, "            edcb5433"));

	mpaland_dbjdbj::sprintf(buffer, "%20.x", 0U);
	REQUIRE(!strcmp(buffer, "                    "));

	mpaland_dbjdbj::sprintf(buffer, "%20.X", 305441741);
	REQUIRE(!strcmp(buffer, "            1234ABCD"));

	mpaland_dbjdbj::sprintf(buffer, "%20.0X", 3989525555U);
	REQUIRE(!strcmp(buffer, "            EDCB5433"));

	mpaland_dbjdbj::sprintf(buffer, "%20.X", 0U);
	REQUIRE(!strcmp(buffer, "                    "));
}


TEST_CASE("float", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%.4f", 3.1415354);
	REQUIRE(!strcmp(buffer, "3.1415"));

	mpaland_dbjdbj::sprintf(buffer, "%.3f", 30343.1415354);
	REQUIRE(!strcmp(buffer, "30343.142"));

	mpaland_dbjdbj::sprintf(buffer, "%.0f", 34.1415354);
	REQUIRE(!strcmp(buffer, "34"));

	mpaland_dbjdbj::sprintf(buffer, "%.2f", 42.8952);
	REQUIRE(!strcmp(buffer, "42.90"));

	mpaland_dbjdbj::sprintf(buffer, "%.9f", 42.8952);
	REQUIRE(!strcmp(buffer, "42.895200000"));

	mpaland_dbjdbj::sprintf(buffer, "%.10f", 42.895223);
	REQUIRE(!strcmp(buffer, "42.8952230000"));

	// this testcase checks, that the precision is truncated to 9 digits.
	// a perfect working float should return the whole number
	mpaland_dbjdbj::sprintf(buffer, "%.12f", 42.89522312345678);
	REQUIRE(!strcmp(buffer, "42.895223123000"));

	// this testcase checks, that the precision is truncated AND rounded to 9 digits.
	// a perfect working float should return the whole number
	mpaland_dbjdbj::sprintf(buffer, "%.12f", 42.89522387654321);
	REQUIRE(!strcmp(buffer, "42.895223877000"));

	mpaland_dbjdbj::sprintf(buffer, "%6.2f", 42.8952);
	REQUIRE(!strcmp(buffer, " 42.90"));

	mpaland_dbjdbj::sprintf(buffer, "%+6.2f", 42.8952);
	REQUIRE(!strcmp(buffer, "+42.90"));

	mpaland_dbjdbj::sprintf(buffer, "%+5.1f", 42.9252);
	REQUIRE(!strcmp(buffer, "+42.9"));

	mpaland_dbjdbj::sprintf(buffer, "%f", 42.5);
	REQUIRE(!strcmp(buffer, "42.500000"));

	mpaland_dbjdbj::sprintf(buffer, "%.1f", 42.5);
	REQUIRE(!strcmp(buffer, "42.5"));

	mpaland_dbjdbj::sprintf(buffer, "%f", 42167.0);
	REQUIRE(!strcmp(buffer, "42167.000000"));

	mpaland_dbjdbj::sprintf(buffer, "%.9f", -12345.987654321);
	REQUIRE(!strcmp(buffer, "-12345.987654321"));

	mpaland_dbjdbj::sprintf(buffer, "%.1f", 3.999);
	REQUIRE(!strcmp(buffer, "4.0"));

	mpaland_dbjdbj::sprintf(buffer, "%.0f", 3.5);
	REQUIRE(!strcmp(buffer, "4"));

	mpaland_dbjdbj::sprintf(buffer, "%.0f", 3.49);
	REQUIRE(!strcmp(buffer, "3"));

	mpaland_dbjdbj::sprintf(buffer, "%.1f", 3.49);
	REQUIRE(!strcmp(buffer, "3.5"));

	// out of range in the moment, need to be fixed by someone
	mpaland_dbjdbj::sprintf(buffer, "%.1f", 1E20);
	REQUIRE(!strcmp(buffer, ""));
}


TEST_CASE("types", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%i", 0);
	REQUIRE(!strcmp(buffer, "0"));

	mpaland_dbjdbj::sprintf(buffer, "%i", 1234);
	REQUIRE(!strcmp(buffer, "1234"));

	mpaland_dbjdbj::sprintf(buffer, "%i", 32767);
	REQUIRE(!strcmp(buffer, "32767"));

	mpaland_dbjdbj::sprintf(buffer, "%i", -32767);
	REQUIRE(!strcmp(buffer, "-32767"));

	mpaland_dbjdbj::sprintf(buffer, "%li", 30L);
	REQUIRE(!strcmp(buffer, "30"));

	mpaland_dbjdbj::sprintf(buffer, "%li", -2147483647L);
	REQUIRE(!strcmp(buffer, "-2147483647"));

	mpaland_dbjdbj::sprintf(buffer, "%li", 2147483647L);
	REQUIRE(!strcmp(buffer, "2147483647"));

	mpaland_dbjdbj::sprintf(buffer, "%lli", 30LL);
	REQUIRE(!strcmp(buffer, "30"));

	mpaland_dbjdbj::sprintf(buffer, "%lli", -9223372036854775807LL);
	REQUIRE(!strcmp(buffer, "-9223372036854775807"));

	mpaland_dbjdbj::sprintf(buffer, "%lli", 9223372036854775807LL);
	REQUIRE(!strcmp(buffer, "9223372036854775807"));

	mpaland_dbjdbj::sprintf(buffer, "%lu", 100000L);
	REQUIRE(!strcmp(buffer, "100000"));

	mpaland_dbjdbj::sprintf(buffer, "%lu", 0xFFFFFFFFL);
	REQUIRE(!strcmp(buffer, "4294967295"));

	mpaland_dbjdbj::sprintf(buffer, "%llu", 281474976710656LLU);
	REQUIRE(!strcmp(buffer, "281474976710656"));

	mpaland_dbjdbj::sprintf(buffer, "%llu", 18446744073709551615LLU);
	REQUIRE(!strcmp(buffer, "18446744073709551615"));

	mpaland_dbjdbj::sprintf(buffer, "%zu", 2147483647UL);
	REQUIRE(!strcmp(buffer, "2147483647"));

	mpaland_dbjdbj::sprintf(buffer, "%zd", 2147483647UL);
	REQUIRE(!strcmp(buffer, "2147483647"));

	if (sizeof(std::size_t) == sizeof(long)) {
		mpaland_dbjdbj::sprintf(buffer, "%zi", -2147483647L);
		REQUIRE(!strcmp(buffer, "-2147483647"));
	}
	else {
		mpaland_dbjdbj::sprintf(buffer, "%zi", -2147483647LL);
		REQUIRE(!strcmp(buffer, "-2147483647"));
	}

	mpaland_dbjdbj::sprintf(buffer, "%b", 60000);
	REQUIRE(!strcmp(buffer, "1110101001100000"));

	mpaland_dbjdbj::sprintf(buffer, "%lb", 12345678L);
	REQUIRE(!strcmp(buffer, "101111000110000101001110"));

	mpaland_dbjdbj::sprintf(buffer, "%o", 60000);
	REQUIRE(!strcmp(buffer, "165140"));

	mpaland_dbjdbj::sprintf(buffer, "%lo", 12345678L);
	REQUIRE(!strcmp(buffer, "57060516"));

	mpaland_dbjdbj::sprintf(buffer, "%lx", 0x12345678L);
	REQUIRE(!strcmp(buffer, "12345678"));

	mpaland_dbjdbj::sprintf(buffer, "%llx", 0x1234567891234567LLU);
	REQUIRE(!strcmp(buffer, "1234567891234567"));

	mpaland_dbjdbj::sprintf(buffer, "%lx", 0xabcdefabL);
	REQUIRE(!strcmp(buffer, "abcdefab"));

	mpaland_dbjdbj::sprintf(buffer, "%lX", 0xabcdefabL);
	REQUIRE(!strcmp(buffer, "ABCDEFAB"));

	mpaland_dbjdbj::sprintf(buffer, "%c", 'v');
	REQUIRE(!strcmp(buffer, "v"));

	mpaland_dbjdbj::sprintf(buffer, "%cv", 'w');
	REQUIRE(!strcmp(buffer, "wv"));

	mpaland_dbjdbj::sprintf(buffer, "%s", "A Test");
	REQUIRE(!strcmp(buffer, "A Test"));

	mpaland_dbjdbj::sprintf(buffer, "%hhu", 0xFFFFUL);
	REQUIRE(!strcmp(buffer, "255"));

	mpaland_dbjdbj::sprintf(buffer, "%hu", 0x123456UL);
	REQUIRE(!strcmp(buffer, "13398"));

	mpaland_dbjdbj::sprintf(buffer, "%s%hhi %hu", "Test", 10000, 0xFFFFFFFF);
	REQUIRE(!strcmp(buffer, "Test16 65535"));

	mpaland_dbjdbj::sprintf(buffer, "%tx", &buffer[10] - &buffer[0]);
	REQUIRE(!strcmp(buffer, "a"));

	// TBD
	if (sizeof(intmax_t) == sizeof(long)) {
		mpaland_dbjdbj::sprintf(buffer, "%ji", -2147483647L);
		REQUIRE(!strcmp(buffer, "-2147483647"));
	}
	else {
		mpaland_dbjdbj::sprintf(buffer, "%ji", -2147483647LL);
		REQUIRE(!strcmp(buffer, "-2147483647"));
	}

}


TEST_CASE("pointer", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%p", (void*)0x1234U);
	if (sizeof(void*) == 4U) {
		REQUIRE(!strcmp(buffer, "00001234"));
	}
	else {
		REQUIRE(!strcmp(buffer, "0000000000001234"));
	}

	mpaland_dbjdbj::sprintf(buffer, "%p", (void*)0x12345678U);
	if (sizeof(void*) == 4U) {
		REQUIRE(!strcmp(buffer, "12345678"));
	}
	else {
		REQUIRE(!strcmp(buffer, "0000000012345678"));
	}

	mpaland_dbjdbj::sprintf(buffer, "%p-%p", (void*)0x12345678U, (void*)0x7EDCBA98U);
	if (sizeof(void*) == 4U) {
		REQUIRE(!strcmp(buffer, "12345678-7EDCBA98"));
	}
	else {
		REQUIRE(!strcmp(buffer, "0000000012345678-000000007EDCBA98"));
	}

	if (sizeof(uintptr_t) == sizeof(uint64_t)) {
		mpaland_dbjdbj::sprintf(buffer, "%p", (void*)(uintptr_t)0xFFFFFFFFU);
		REQUIRE(!strcmp(buffer, "00000000FFFFFFFF"));
	}
	else {
		mpaland_dbjdbj::sprintf(buffer, "%p", (void*)(uintptr_t)0xFFFFFFFFU);
		REQUIRE(!strcmp(buffer, "FFFFFFFF"));
	}
}


TEST_CASE("unknown flag", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%kmarco", 42, 37);
	REQUIRE(!strcmp(buffer, "kmarco"));
}


TEST_CASE("buffer length", "[]") {
	char buffer[100];
	int ret;

	ret = mpaland_dbjdbj::snprintf(nullptr, 10, "%s", "Test");
	REQUIRE(ret == 4);
	ret = mpaland_dbjdbj::snprintf(nullptr, 0, "%s", "Test");
	REQUIRE(ret == 4);

	buffer[0] = (char)0xA5;
	ret = mpaland_dbjdbj::snprintf(buffer, 0, "%s", "Test");
	REQUIRE(buffer[0] == (char)0xA5);
	REQUIRE(ret == 4);

	buffer[0] = (char)0xCC;
	mpaland_dbjdbj::snprintf(buffer, 1, "%s", "Test");
	REQUIRE(buffer[0] == '\0');

	mpaland_dbjdbj::snprintf(buffer, 2, "%s", "Hello");
	REQUIRE(!strcmp(buffer, "H"));
}


TEST_CASE("ret value", "[]") {
	char buffer[100];
	int ret;

	ret = mpaland_dbjdbj::snprintf(buffer, 6, "0%s", "1234");
	REQUIRE(!strcmp(buffer, "01234"));
	REQUIRE(ret == 5);

	ret = mpaland_dbjdbj::snprintf(buffer, 6, "0%s", "12345");
	REQUIRE(!strcmp(buffer, "01234"));
	REQUIRE(ret == 6);  // '5' is truncated

	ret = mpaland_dbjdbj::snprintf(buffer, 6, "0%s", "1234567");
	REQUIRE(!strcmp(buffer, "01234"));
	REQUIRE(ret == 8);  // '567' are truncated

	ret = mpaland_dbjdbj::snprintf(buffer, 10, "hello, world");
	REQUIRE(ret == 12);

	ret = mpaland_dbjdbj::snprintf(buffer, 3, "%d", 10000);
	REQUIRE(ret == 5);
	REQUIRE(strlen(buffer) == 2U);
	REQUIRE(buffer[0] == '1');
	REQUIRE(buffer[1] == '0');
	REQUIRE(buffer[2] == '\0');
}


TEST_CASE("misc", "[]") {
	char buffer[100];

	mpaland_dbjdbj::sprintf(buffer, "%u%u%ctest%d %s", 5, 3000, 'a', -20, "bit");
	REQUIRE(!strcmp(buffer, "53000atest-20 bit"));

	mpaland_dbjdbj::sprintf(buffer, "%.*f", 2, 0.33333333);
	REQUIRE(!strcmp(buffer, "0.33"));

	mpaland_dbjdbj::sprintf(buffer, "%.3s", "foobar");
	REQUIRE(!strcmp(buffer, "foo"));

	mpaland_dbjdbj::sprintf(buffer, "%10.5d", 4);
	REQUIRE(!strcmp(buffer, "     00004"));

	mpaland_dbjdbj::sprintf(buffer, "%*sx", -3, "hi");
	REQUIRE(!strcmp(buffer, "hi x"));
}


} // namespace