# wprintf++

### This is WIN32, wchar_t version of the [printf++ project](https://github.com/DBJDBJ/printf_plusplus)

### This is a light C++ transformation from the original project of [Marco Paland](https://github.com/mpaland/printf)

#### C++ header only version
#### Please see the core ideas in the original [project](https://github.com/mpaland/printf).

### Highligths and design goals of this version

- Header only C++17 version of [printf](https://github.com/mpaland/printf)
- Immediately usable in any C++17 project.
- Use of ```wchar_t``` for ```WIN32``` projects


### Usage

Include *wprintf++.h*. That's it. Everything is in the ```mpaland_dbjdbj::wchar``` namespace.

```C++
namespace {
   using mpaland_dbjdbj::wchar::printf ;
   printf(L"\nHello %s!", L"world");
}
```
If you ```#define PRINTF_USER_DEFINED_PUTCHAR```, you will need to implement your low level output function needed for `printf()`:
```C++
namespace mpaland_dbjdbj::wchar {
  inline void _putchar(wchar_t character)
	{
	// your code here
	}
}
```
Otherwise canonical implementation is provided:
```C++
	inline void _putchar(wchar_t character) {
		std::putwchar(character);
	}
```
Usage is like the stdio.h versions. The only difference is the ```mpaland_dbjdbj``` namespace: _
```C++
namespace mpaland_dbjdbj {
int printf(const wchar_t* format, ...);
int sprintf(wchar_t* buffer, const wchar_t* format, ...);
int snprintf(wchar_t* buffer, size_t count, const wchar_t* format, ...);
int vsnprintf(wchar_t* buffer, size_t count, const wchar_t* format, va_list va);
int fctprintf(void (*out)(char character, void* arg), void* arg, const wchar_t* format, ...);
}
```


## Test suite
For testing just compile, build and run the test suite located in `test/cpp_test_suite.cpp`. That uses the [catch](https://github.com/catchorg/Catch2) framework for unit-tests, which is auto-adding main().  
This is  VS2017/C+17 solution.

## License
printf,printf++ and wprintf++ are written under the [MIT license](http://www.opensource.org/licenses/MIT).
