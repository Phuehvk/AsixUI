// AsixUI Template Library - AsixUI version 1.0
// Copyright (C)  2013 Kusiv Inc.
// All rights reserved.

#ifndef _ASIXAPP_INC_
#define _ASIXAPP_INC_

#pragma once

#ifndef __cplusplus
#error AsixUI requires C++ compilation (use a .cpp suffix)
#endif

#include <asixver.h>
#include <atlbase.h>

#if (_MSC_VER < 1500)
#error AsixUI requires Visual C++ 2008 compiler or higher
#endif

#ifndef __ATLBASE_H__
#error asixapp.h requires atlbase.h to be included first
#endif

#ifdef defined(_WIN32_WCE) 
#error AsixUI  is not supported WinCE platform
#endif

#if (_WIN32_WINNT < 0x0501 || _RICHEDIT_VER < 0x0300)
#error AsixUI requires Windows XP or higher
#endif

#ifdef _ATL_NO_COMMODULE
#error AsixUI requires that _ATL_NO_COMMODULE is not defined
#endif

#if (_ATL_VER < 0x0900)
#error  AsixUI requires ATL 9.0 or higher
#elif defined(_ATL_MIN_CRT)
#error _ATL_MIN_CRT is not supported with ATL 9.0 or higher
#endif


#include <limits.h>
#if !defined(_ATL_MIN_CRT) && defined(_MT)
#include <process.h>	// for _beginthreadex
#endif

#include <commctrl.h>
// #pragma comment(lib, "comctl32.lib")
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <asixres.h>

// We need to disable this warning because of template class arguments
#pragma warning(disable: 4127)

#if (_ATL_VER >= 0x0900) && !defined(_SECURE_ATL)
#define _SECURE_ATL	1
#endif



///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMessageFilter
// CIdleHandler
// CMessageLoop
//
// CAppModule
// CServerAppModule
//
// CRegKeyEx
//
// Global functions:
//   AtlGetDefaultGuiFont()
//   AtlCreateControlFont()
//   AtlCreateBoldFont()
//   AtlInitCommonControls()


///////////////////////////////////////////////////////////////////////////////
// Global support for SecureHelper functions

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifndef _ERRCODE_DEFINED
#define _ERRCODE_DEFINED
typedef int errno_t;
#endif

#ifndef _SECURECRT_ERRCODE_VALUES_DEFINED
#define _SECURECRT_ERRCODE_VALUES_DEFINED
#define EINVAL          22
#define STRUNCATE       80
#endif

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif


///////////////////////////////////////////////////////////////////////////////
// Miscellaneous global support

// define useful macros from winuser.h
#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(_r) (((ULONG_PTR)(_r) >> 16) == 0)
#endif // IS_INTRESOURCE

// protect template members from windowsx.h macros
#ifdef _INC_WINDOWSX
#undef SubclassWindow
#endif // _INC_WINDOWSX

// define useful macros from windowsx.h
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

// Dummy structs for compiling with /CLR
#if (_MSC_VER < 1500)
#error AsixUI requires VC9 or higher
#elif defined(_MANAGED)
__if_not_exists(_IMAGELIST::_IMAGELIST) { struct _IMAGELIST { }; }
__if_not_exists(_TREEITEM::_TREEITEM) { struct _TREEITEM { }; }
__if_not_exists(_PSP::_PSP) { struct _PSP { }; }
#endif


// Forward declaration for ATL11 fix
#if (_ATL_VER >= 0x0B00)
namespace ATL { HRESULT AtlGetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor); }
#endif

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL                  0x020E
#endif


namespace Asix
{

	DECLARE_TRACE_CATEGORY(atlTraceUI);
	#ifdef _DEBUG
	__declspec(selectany) ATL::CTraceCategory atlTraceUI(_T("atlTraceUI"));
	#endif // _DEBUG

	// Windows version helper
	inline bool AtlIsOldWindows()
	{
		OSVERSIONINFO ovi = { sizeof(OSVERSIONINFO) };
		BOOL bRet = ::GetVersionEx(&ovi);
		return (!bRet || !(ovi.dwMajorVersion >= 6 || (ovi.dwMajorVersion >= 5 && ovi.dwMinorVersion >= 1)));
	}

	// Default GUI font helper - "MS Shell Dlg" stock font
	inline HFONT AtlGetDefaultGuiFont()
	{
		return (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}

	// Control font helper - default font for controls not in a dialog
	// (NOTE: Caller owns the font, and should destroy it when it's no longer needed)
	inline HFONT AtlCreateControlFont()
	{
		LOGFONT lf = { 0 };
		ATLVERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0) != FALSE);
		HFONT hFont = ::CreateFontIndirect(&lf);
		ATLASSERT(hFont != NULL);
		return hFont;
	}

	// Bold font helper
	// (NOTE: Caller owns the font, and should destroy it when it's no longer needed)
	inline HFONT AtlCreateBoldFont(HFONT hFont = NULL)
	{
		LOGFONT lf = { 0 };
		if(hFont == NULL)
			ATLVERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0) != FALSE);
		else
			ATLVERIFY(::GetObject(hFont, sizeof(LOGFONT), &lf) == sizeof(LOGFONT));
		lf.lfWeight = FW_BOLD;
		HFONT hFontBold =  ::CreateFontIndirect(&lf);
		ATLASSERT(hFontBold != NULL);
		return hFontBold;
	}

	// Common Controls initialization helper
	inline BOOL AtlInitCommonControls(DWORD dwFlags)
	{
		INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), dwFlags };
		BOOL bRet = ::InitCommonControlsEx(&iccx);
		ATLASSERT(bRet);
		return bRet;
	}


	///////////////////////////////////////////////////////////////////////////////
	// RunTimeHelper - helper functions for Windows version and structure sizes


	#ifndef _ASIX_NO_RUNTIME_STRUCT_SIZE

	#ifndef _SIZEOF_STRUCT
	#define _SIZEOF_STRUCT(structname, member)  (((int)((LPBYTE)(&((structname*)0)->member) - ((LPBYTE)((structname*)0)))) + sizeof(((structname*)0)->member))
	#endif

	#if (_WIN32_WINNT >= 0x0600) && !defined(REBARBANDINFO_V6_SIZE)
	#define REBARBANDINFO_V6_SIZE   _SIZEOF_STRUCT(REBARBANDINFO, cxHeader)
	#endif // (_WIN32_WINNT >= 0x0600) && !defined(REBARBANDINFO_V6_SIZE)

	#if (_WIN32_WINNT >= 0x0600) && !defined(LVGROUP_V5_SIZE)
	#define LVGROUP_V5_SIZE   _SIZEOF_STRUCT(LVGROUP, uAlign)
	#endif // (_WIN32_WINNT >= 0x0600) && !defined(LVGROUP_V5_SIZE)

	#if (_WIN32_WINNT >= 0x0600) && !defined(LVTILEINFO_V5_SIZE)
	#define LVTILEINFO_V5_SIZE   _SIZEOF_STRUCT(LVTILEINFO, puColumns)
	#endif // (_WIN32_WINNT >= 0x0600) && !defined(LVTILEINFO_V5_SIZE)

	#if defined(NTDDI_VERSION) && (NTDDI_VERSION >= NTDDI_LONGHORN) && !defined(MCHITTESTINFO_V1_SIZE)
	#define MCHITTESTINFO_V1_SIZE   _SIZEOF_STRUCT(MCHITTESTINFO, st)
	#endif // defined(NTDDI_VERSION) && (NTDDI_VERSION >= NTDDI_LONGHORN) && !defined(MCHITTESTINFO_V1_SIZE)

	#if (WINVER >= 0x0600) && !defined(NONCLIENTMETRICS_V1_SIZE)
	#define NONCLIENTMETRICS_V1_SIZE   _SIZEOF_STRUCT(NONCLIENTMETRICS, lfMessageFont)
	#endif // !defined(_WIN32_WCE) && (WINVER >= 0x0600) && !defined(NONCLIENTMETRICS_V1_SIZE)

	#endif // !_ASIX_NO_RUNTIME_STRUCT_SIZE

	namespace RunTimeHelper
	{
		inline bool IsCommCtrl6()
		{
			DWORD dwMajor = 0, dwMinor = 0;
			HRESULT hRet = ATL::AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
			return (SUCCEEDED(hRet) && (dwMajor >= 6));
		}

		inline bool IsVista()
		{
			OSVERSIONINFO ovi = { sizeof(OSVERSIONINFO) };
			BOOL bRet = ::GetVersionEx(&ovi);
			return ((bRet != FALSE) && (ovi.dwMajorVersion >= 6));
		}

		inline bool IsThemeAvailable()
		{
			bool bRet = false;

			if(IsCommCtrl6())
			{
				HMODULE hThemeDLL = ::LoadLibrary(_T("uxtheme.dll"));
				if(hThemeDLL != NULL)
				{
					typedef BOOL (STDAPICALLTYPE *PFN_IsThemeActive)();
					PFN_IsThemeActive pfnIsThemeActive = (PFN_IsThemeActive)::GetProcAddress(hThemeDLL, "IsThemeActive");
					ATLASSERT(pfnIsThemeActive != NULL);
					bRet = (pfnIsThemeActive != NULL) && (pfnIsThemeActive() != FALSE);
					if(bRet)
					{
						typedef BOOL (STDAPICALLTYPE *PFN_IsAppThemed)();
						PFN_IsAppThemed pfnIsAppThemed = (PFN_IsAppThemed)::GetProcAddress(hThemeDLL, "IsAppThemed");
						ATLASSERT(pfnIsAppThemed != NULL);
						bRet = (pfnIsAppThemed != NULL) && (pfnIsAppThemed() != FALSE);
					}

					::FreeLibrary(hThemeDLL);
				}
			}
			return bRet;
		}

		inline bool IsWin7()
		{
			OSVERSIONINFO ovi = { sizeof(OSVERSIONINFO) };
			BOOL bRet = ::GetVersionEx(&ovi);
			return ((bRet != FALSE) && (ovi.dwMajorVersion == 6) && (ovi.dwMinorVersion >= 1));
		}

		inline bool IsRibbonUIAvailable()
		{
			static INT iRibbonUI = -1;

			#if defined(NTDDI_WIN7) && (NTDDI_VERSION >= NTDDI_WIN7)
			if (iRibbonUI == -1)
			{
				HMODULE hRibbonDLL = ::LoadLibrary(_T("propsys.dll"));
				if (hRibbonDLL != NULL)
				{
					const GUID CLSID_UIRibbonFramework = { 0x926749fa, 0x2615, 0x4987, { 0x88, 0x45, 0xc3, 0x3e, 0x65, 0xf2, 0xb9, 0x57 } };
					// block - create instance
					{
						ATL::CComPtr<IUnknown> pIUIFramework;
						iRibbonUI = SUCCEEDED(pIUIFramework.CoCreateInstance(CLSID_UIRibbonFramework)) ? 1 : 0;
					}
					::FreeLibrary(hRibbonDLL);
				}
				else
				{
					iRibbonUI = 0;
				}
			}
			#endif // defined(NTDDI_WIN7) && (NTDDI_VERSION >= NTDDI_WIN7)

			return (iRibbonUI == 1);
		}


		inline int SizeOf_REBARBANDINFO()
		{
			int nSize = sizeof(REBARBANDINFO);
			#if !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (_WIN32_WINNT >= 0x0600)
			if(!(IsVista() && IsCommCtrl6()))
				nSize = REBARBANDINFO_V6_SIZE;
			#endif // !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (_WIN32_WINNT >= 0x0600)
			return nSize;
		}

		inline int SizeOf_LVGROUP()
		{
			int nSize = sizeof(LVGROUP);
			#if !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (_WIN32_WINNT >= 0x0600)
			if(!IsVista())
				nSize = LVGROUP_V5_SIZE;
			#endif // !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (_WIN32_WINNT >= 0x0600)
			return nSize;
		}

		inline int SizeOf_LVTILEINFO()
		{
			int nSize = sizeof(LVTILEINFO);
			#if !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (_WIN32_WINNT >= 0x0600)
			if(!IsVista())
				nSize = LVTILEINFO_V5_SIZE;
			#endif // !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (_WIN32_WINNT >= 0x0600)
			return nSize;
		}

		inline int SizeOf_MCHITTESTINFO()
		{
			int nSize = sizeof(MCHITTESTINFO);
			#if !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && defined(NTDDI_VERSION) && (NTDDI_VERSION >= NTDDI_LONGHORN)
			if(!(IsVista() && IsCommCtrl6()))
				nSize = MCHITTESTINFO_V1_SIZE;
			#endif // !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && defined(NTDDI_VERSION) && (NTDDI_VERSION >= NTDDI_LONGHORN)
			return nSize;
		}

		inline int SizeOf_NONCLIENTMETRICS()
		{
			int nSize = sizeof(NONCLIENTMETRICS);
			#if !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (WINVER >= 0x0600)
			if(!IsVista())
				nSize = NONCLIENTMETRICS_V1_SIZE;
			#endif // !defined(_ASIX_NO_RUNTIME_STRUCT_SIZE) && (WINVER >= 0x0600)
			return nSize;
		}
	}	// namespace RunTimeHelper


	///////////////////////////////////////////////////////////////////////////////
	// ModuleHelper - helper functions for ATL3 and ATL7 module classes

	namespace ModuleHelper
	{
		inline HINSTANCE GetModuleInstance()
		{
			return ATL::_AtlBaseModule.GetModuleInstance();
		}

		inline HINSTANCE GetResourceInstance()
		{
			return ATL::_AtlBaseModule.GetResourceInstance();
		}

		inline void AddCreateWndData(ATL::_AtlCreateWndData* pData, void* pObject)
		{
			ATL::_AtlWinModule.AddCreateWndData(pData, pObject);
		}

		inline void* ExtractCreateWndData()
		{
			return ATL::_AtlWinModule.ExtractCreateWndData();
		}
	}	// namespace ModuleHelper


	///////////////////////////////////////////////////////////////////////////////
	// SecureHelper - helper functions for VS2005 secure CRT

	namespace SecureHelper
	{
		inline void strcpyA_x(char* lpstrDest, size_t cchDest, const char* lpstrSrc)
		{
			#if _SECURE_ATL
			ATL::Checked::strcpy_s(lpstrDest, cchDest, lpstrSrc);
			#else
			if(cchDest > (size_t)lstrlenA(lpstrSrc))
				ATLVERIFY(lstrcpyA(lpstrDest, lpstrSrc) != NULL);
			else
				ATLASSERT(FALSE);
			#endif
		}

		inline void strcpyW_x(wchar_t* lpstrDest, size_t cchDest, const wchar_t* lpstrSrc)
		{
			#if _SECURE_ATL
			ATL::Checked::wcscpy_s(lpstrDest, cchDest, lpstrSrc);
			#else
			if(cchDest > (size_t)lstrlenW(lpstrSrc))
				ATLVERIFY(lstrcpyW(lpstrDest, lpstrSrc) != NULL);
			else
				ATLASSERT(FALSE);
			#endif
		}

		inline void strcpy_x(LPTSTR lpstrDest, size_t cchDest, LPCTSTR lpstrSrc)
		{
			#ifdef _UNICODE
			strcpyW_x(lpstrDest, cchDest, lpstrSrc);
			#else
			strcpyA_x(lpstrDest, cchDest, lpstrSrc);
			#endif
		}

		inline errno_t strncpyA_x(char* lpstrDest, size_t cchDest, const char* lpstrSrc, size_t cchCount)
		{
			#if _SECURE_ATL
			return ATL::Checked::strncpy_s(lpstrDest, cchDest, lpstrSrc, cchCount);
			#else
			errno_t nRet = 0;
			if(lpstrDest == NULL || cchDest == 0 || lpstrSrc == NULL)
			{
				nRet = EINVAL;
			}
			else if(cchCount == _TRUNCATE)
			{
				cchCount = min(cchDest - 1, size_t(lstrlenA(lpstrSrc)));
				nRet = STRUNCATE;
			}
			else if(cchDest <= cchCount)
			{
				lpstrDest[0] = 0;
				nRet = EINVAL;
			}
			if(nRet == 0 || nRet == STRUNCATE)
				nRet = (lstrcpynA(lpstrDest, lpstrSrc, (int)cchCount + 1) != NULL) ? nRet : EINVAL;
			ATLASSERT(nRet == 0 || nRet == STRUNCATE);
			return nRet;
			#endif
		}

		inline errno_t strncpyW_x(wchar_t* lpstrDest, size_t cchDest, const wchar_t* lpstrSrc, size_t cchCount)
		{
			#if _SECURE_ATL
			return ATL::Checked::wcsncpy_s(lpstrDest, cchDest, lpstrSrc, cchCount);
			#else
			errno_t nRet = 0;
			if(lpstrDest == NULL || cchDest == 0 || lpstrSrc == NULL)
			{
				nRet = EINVAL;
			}
			else if(cchCount == _TRUNCATE)
			{
				cchCount = min(cchDest - 1, size_t(lstrlenW(lpstrSrc)));
				nRet = STRUNCATE;
			}
			else if(cchDest <= cchCount)
			{
				lpstrDest[0] = 0;
				nRet = EINVAL;
			}
			if(nRet == 0 || nRet == STRUNCATE)
				nRet = (lstrcpynW(lpstrDest, lpstrSrc, (int)cchCount + 1) != NULL) ? nRet : EINVAL;
			ATLASSERT(nRet == 0 || nRet == STRUNCATE);
			return nRet;
			#endif
		}

		inline errno_t strncpy_x(LPTSTR lpstrDest, size_t cchDest, LPCTSTR lpstrSrc, size_t cchCount)
		{
			#ifdef _UNICODE
			return strncpyW_x(lpstrDest, cchDest, lpstrSrc, cchCount);
			#else
			return strncpyA_x(lpstrDest, cchDest, lpstrSrc, cchCount);
			#endif
		}

		inline void strcatA_x(char* lpstrDest, size_t cchDest, const char* lpstrSrc)
		{
			#if _SECURE_ATL
			ATL::Checked::strcat_s(lpstrDest, cchDest, lpstrSrc);
			#else
			if(cchDest > (size_t)lstrlenA(lpstrSrc))
				ATLVERIFY(lstrcatA(lpstrDest, lpstrSrc) != NULL);
			else
				ATLASSERT(FALSE);
			#endif
		}

		inline void strcatW_x(wchar_t* lpstrDest, size_t cchDest, const wchar_t* lpstrSrc)
		{
			#if _SECURE_ATL
			ATL::Checked::wcscat_s(lpstrDest, cchDest, lpstrSrc);
			#else
			if(cchDest > (size_t)lstrlenW(lpstrSrc))
				ATLVERIFY(lstrcatW(lpstrDest, lpstrSrc) != NULL);
			else
				ATLASSERT(FALSE);
			#endif
		}

		inline void strcat_x(LPTSTR lpstrDest, size_t cchDest, LPCTSTR lpstrSrc)
		{
			#ifdef _UNICODE
			strcatW_x(lpstrDest, cchDest, lpstrSrc);
			#else
			strcatA_x(lpstrDest, cchDest, lpstrSrc);
			#endif
		}

		inline void memcpy_x(void* pDest, size_t cbDest, const void* pSrc, size_t cbSrc)
		{
			#if _SECURE_ATL
			ATL::Checked::memcpy_s(pDest, cbDest, pSrc, cbSrc);
			#else
			if(cbDest >= cbSrc)
				memcpy(pDest, pSrc, cbSrc);
			else
				ATLASSERT(FALSE);
			#endif
		}

		inline void memmove_x(void* pDest, size_t cbDest, const void* pSrc, size_t cbSrc)
		{
			#if _SECURE_ATL
			ATL::Checked::memmove_s(pDest, cbDest, pSrc, cbSrc);
			#else
			if(cbDest >= cbSrc)
				memmove(pDest, pSrc, cbSrc);
			else
				ATLASSERT(FALSE);
			#endif
		}

		inline int vsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, va_list args)
		{
			#if _SECURE_ATL && !defined(_ATL_MIN_CRT)
			return _vstprintf_s(lpstrBuff, cchBuff, lpstrFormat, args);
			#else
			cchBuff;   // Avoid unused argument warning
			#pragma warning(disable: 4996)
			return _vstprintf(lpstrBuff, lpstrFormat, args);
			#pragma warning(default: 4996)
			#endif
		}

		inline int wvsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, va_list args)
		{
			#if _SECURE_ATL && !defined(_ATL_MIN_CRT)
			return _vstprintf_s(lpstrBuff, cchBuff, lpstrFormat, args);
			#else
			cchBuff;   // Avoid unused argument warning
			return ::wvsprintf(lpstrBuff, lpstrFormat, args);
			#endif
		}

		inline int sprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, ...)
		{
			va_list args;
			va_start(args, lpstrFormat);
			int nRes = vsprintf_x(lpstrBuff, cchBuff, lpstrFormat, args);
			va_end(args);
			return nRes;
		}

		inline int wsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, ...)
		{
			va_list args;
			va_start(args, lpstrFormat);
			int nRes = wvsprintf_x(lpstrBuff, cchBuff, lpstrFormat, args);
			va_end(args);
			return nRes;
		}
	}	// namespace SecureHelper


	///////////////////////////////////////////////////////////////////////////////
	// MinCrtHelper - helper functions for using _ATL_MIN_CRT

	namespace MinCrtHelper
	{
		inline int _isspace(TCHAR ch)
		{
			#ifndef _ATL_MIN_CRT
			return _istspace(ch);
			#else // _ATL_MIN_CRT
			WORD type = 0;
			::GetStringTypeEx(::GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
			return (type & C1_SPACE) == C1_SPACE;
			#endif // _ATL_MIN_CRT
		}

		inline int _isdigit(TCHAR ch)
		{
			#ifndef _ATL_MIN_CRT
			return _istdigit(ch);
			#else // _ATL_MIN_CRT
			WORD type = 0;
			::GetStringTypeEx(::GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
			return (type & C1_DIGIT) == C1_DIGIT;
			#endif // _ATL_MIN_CRT
		}

		inline int _atoi(LPCTSTR str)
		{
			#ifndef _ATL_MIN_CRT
			return _ttoi(str);
			#else // _ATL_MIN_CRT
			while(_isspace(*str) != 0)
				++str;

			TCHAR ch = *str++;
			TCHAR sign = ch;   // save sign indication
			if(ch == _T('-') || ch == _T('+'))
				ch = *str++;   // skip sign

			int total = 0;
			while(_isdigit(ch) != 0)
			{
				total = 10 * total + (ch - '0');   // accumulate digit
				ch = *str++;        // get next char
			}

			return (sign == '-') ? -total : total;   // return result, negated if necessary
			#endif // _ATL_MIN_CRT
		}

		inline LPCTSTR _strrchr(LPCTSTR str, TCHAR ch)
		{
			#ifndef _ATL_MIN_CRT
			return _tcsrchr(str, ch);
			#else // _ATL_MIN_CRT
			LPCTSTR lpsz = NULL;
			while(*str != 0)
			{
				if(*str == ch)
					lpsz = str;
				str = ::CharNext(str);
			}
			return lpsz;
			#endif // _ATL_MIN_CRT
		}

		inline LPTSTR _strrchr(LPTSTR str, TCHAR ch)
		{
			#ifndef _ATL_MIN_CRT
			return _tcsrchr(str, ch);
			#else // _ATL_MIN_CRT
			LPTSTR lpsz = NULL;
			while(*str != 0)
			{
				if(*str == ch)
					lpsz = str;
				str = ::CharNext(str);
			}
			return lpsz;
			#endif // _ATL_MIN_CRT
		}
	};	// namespace MinCrtHelper


	///////////////////////////////////////////////////////////////////////////////
	// CMessageFilter - Interface for message filter support

	class CMessageFilter
	{
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg) = 0;
	};


	///////////////////////////////////////////////////////////////////////////////
	// CIdleHandler - Interface for idle processing

	class CIdleHandler
	{
	public:
		virtual BOOL OnIdle() = 0;
	};

	#ifndef _ATL_NO_OLD_NAMES
	// for compatilibility with old names only
	typedef CIdleHandler CUpdateUIObject;
	#define DoUpdate OnIdle
	#endif // !_ATL_NO_OLD_NAMES


	///////////////////////////////////////////////////////////////////////////////
	// CMessageLoop - message loop implementation

	class CMessageLoop
	{
	public:
		ATL::CSimpleArray<CMessageFilter*> m_aMsgFilter;
		ATL::CSimpleArray<CIdleHandler*> m_aIdleHandler;
		MSG m_msg;

		// Message filter operations
		BOOL AddMessageFilter(CMessageFilter* pMessageFilter)
		{
			return m_aMsgFilter.Add(pMessageFilter);
		}

		BOOL RemoveMessageFilter(CMessageFilter* pMessageFilter)
		{
			return m_aMsgFilter.Remove(pMessageFilter);
		}

		// Idle handler operations
		BOOL AddIdleHandler(CIdleHandler* pIdleHandler)
		{
			return m_aIdleHandler.Add(pIdleHandler);
		}

		BOOL RemoveIdleHandler(CIdleHandler* pIdleHandler)
		{
			return m_aIdleHandler.Remove(pIdleHandler);
		}

		#ifndef _ATL_NO_OLD_NAMES
		// for compatilibility with old names only
		BOOL AddUpdateUI(CIdleHandler* pIdleHandler)
		{
			ATLTRACE2(atlTraceUI, 0, _T("CUpdateUIObject and AddUpdateUI are deprecated. Please change your code to use CIdleHandler and OnIdle\n"));
			return AddIdleHandler(pIdleHandler);
		}

		BOOL RemoveUpdateUI(CIdleHandler* pIdleHandler)
		{
			ATLTRACE2(atlTraceUI, 0, _T("CUpdateUIObject and RemoveUpdateUI are deprecated. Please change your code to use CIdleHandler and OnIdle\n"));
			return RemoveIdleHandler(pIdleHandler);
		}
		#endif // !_ATL_NO_OLD_NAMES

		// message loop
		int Run()
		{
			BOOL bDoIdle = TRUE;
			int nIdleCount = 0;
			BOOL bRet;

			for(;;)
			{
				while(bDoIdle && !::PeekMessage(&m_msg, NULL, 0, 0, PM_NOREMOVE))
				{
					if(!OnIdle(nIdleCount++))
						bDoIdle = FALSE;
				}

				bRet = ::GetMessage(&m_msg, NULL, 0, 0);

				if(bRet == -1)
				{
					ATLTRACE2(atlTraceUI, 0, _T("::GetMessage returned -1 (error)\n"));
					continue;   // error, don't process
				}
				else if(!bRet)
				{
					ATLTRACE2(atlTraceUI, 0, _T("CMessageLoop::Run - exiting\n"));
					break;   // WM_QUIT, exit message loop
				}

				if(!PreTranslateMessage(&m_msg))
				{
					::TranslateMessage(&m_msg);
					::DispatchMessage(&m_msg);
				}

				if(IsIdleMessage(&m_msg))
				{
					bDoIdle = TRUE;
					nIdleCount = 0;
				}
			}

			return (int)m_msg.wParam;
		}

		static BOOL IsIdleMessage(MSG* pMsg)
		{
			// These messages should NOT cause idle processing
			switch(pMsg->message)
			{
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
			case WM_PAINT:
			case 0x0118:	// WM_SYSTIMER (caret blink)
				return FALSE;
			}

			return TRUE;
		}

		// Overrideables
		// Override to change message filtering
		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			// loop backwards
			for(int i = m_aMsgFilter.GetSize() - 1; i >= 0; i--)
			{
				CMessageFilter* pMessageFilter = m_aMsgFilter[i];
				if(pMessageFilter != NULL && pMessageFilter->PreTranslateMessage(pMsg))
					return TRUE;
			}
			return FALSE;   // not translated
		}

		// override to change idle processing
		virtual BOOL OnIdle(int /*nIdleCount*/)
		{
			for(int i = 0; i < m_aIdleHandler.GetSize(); i++)
			{
				CIdleHandler* pIdleHandler = m_aIdleHandler[i];
				if(pIdleHandler != NULL)
					pIdleHandler->OnIdle();
			}
			return FALSE;   // don't continue
		}
	};


	///////////////////////////////////////////////////////////////////////////////
	// CStaticDataInitCriticalSectionLock and CWindowCreateCriticalSectionLock
	// internal classes to manage critical sections for both ATL3 and ATL7

	class CStaticDataInitCriticalSectionLock
	{
	public:
		ATL::CComCritSecLock<ATL::CComCriticalSection> m_cslock;

		CStaticDataInitCriticalSectionLock() : m_cslock(ATL::_pAtlModule->m_csStaticDataInitAndTypeInfo, false)
		{ }

		HRESULT Lock()
		{
			return m_cslock.Lock();
		}

		void Unlock()
		{
			m_cslock.Unlock();
		}
	};


	class CWindowCreateCriticalSectionLock
	{
	public:
		ATL::CComCritSecLock<ATL::CComCriticalSection> m_cslock;

		CWindowCreateCriticalSectionLock() : m_cslock(ATL::_AtlWinModule.m_csWindowCreate, false)
		{ }

		HRESULT Lock()
		{
			return m_cslock.Lock();
		}

		void Unlock()
		{
			m_cslock.Unlock();
		}
	};


	///////////////////////////////////////////////////////////////////////////////
	// CTempBuffer - helper class for stack allocations for ATL3

	#ifndef _ASIX_STACK_ALLOC_THRESHOLD
	#define _ASIX_STACK_ALLOC_THRESHOLD   512
	#endif

	using ATL::CTempBuffer;


	///////////////////////////////////////////////////////////////////////////////
	// CAppModule - module class for an application

	class CAppModule : public ATL::CComModule
	{
	public:
		DWORD m_dwMainThreadID;
		ATL::CSimpleMap<DWORD, CMessageLoop*>* m_pMsgLoopMap;
		ATL::CSimpleArray<HWND>* m_pSettingChangeNotify;

		// Overrides of CComModule::Init and Term
		HRESULT Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL)
		{
			HRESULT hRet = CComModule::Init(pObjMap, hInstance, pLibID);
			if(FAILED(hRet))
				return hRet;

			m_dwMainThreadID = ::GetCurrentThreadId();
			typedef ATL::CSimpleMap<DWORD, CMessageLoop*>   _mapClass;
			m_pMsgLoopMap = NULL;
			ATLTRY(m_pMsgLoopMap = new _mapClass);
			if(m_pMsgLoopMap == NULL)
				return E_OUTOFMEMORY;
			m_pSettingChangeNotify = NULL;

			return hRet;
		}

		void Term()
		{
			TermSettingChangeNotify();
			delete m_pMsgLoopMap;
			CComModule::Term();
		}

		// Message loop map methods
		BOOL AddMessageLoop(CMessageLoop* pMsgLoop)
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::AddMessageLoop.\n"));
				ATLASSERT(FALSE);
				return FALSE;
			}

			ATLASSERT(pMsgLoop != NULL);
			ATLASSERT(m_pMsgLoopMap->Lookup(::GetCurrentThreadId()) == NULL);   // not in map yet

			BOOL bRet = m_pMsgLoopMap->Add(::GetCurrentThreadId(), pMsgLoop);

			lock.Unlock();

			return bRet;
		}

		BOOL RemoveMessageLoop()
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::RemoveMessageLoop.\n"));
				ATLASSERT(FALSE);
				return FALSE;
			}

			BOOL bRet = m_pMsgLoopMap->Remove(::GetCurrentThreadId());

			lock.Unlock();

			return bRet;
		}

		CMessageLoop* GetMessageLoop(DWORD dwThreadID = ::GetCurrentThreadId()) const
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::GetMessageLoop.\n"));
				ATLASSERT(FALSE);
				return NULL;
			}

			CMessageLoop* pLoop =  m_pMsgLoopMap->Lookup(dwThreadID);

			lock.Unlock();

			return pLoop;
		}

		// Setting change notify methods
		// Note: Call this from the main thread for MSDI apps
		BOOL InitSettingChangeNotify(DLGPROC pfnDlgProc = _SettingChangeDlgProc)
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::InitSettingChangeNotify.\n"));
				ATLASSERT(FALSE);
				return FALSE;
			}

			if(m_pSettingChangeNotify == NULL)
			{
				typedef ATL::CSimpleArray<HWND>   _notifyClass;
				ATLTRY(m_pSettingChangeNotify = new _notifyClass);
				ATLASSERT(m_pSettingChangeNotify != NULL);
			}

			BOOL bRet = (m_pSettingChangeNotify != NULL);
			if(bRet && m_pSettingChangeNotify->GetSize() == 0)
			{
				// init everything
				_ATL_EMPTY_DLGTEMPLATE templ;
				HWND hNtfWnd = ::CreateDialogIndirect(GetModuleInstance(), &templ, NULL, pfnDlgProc);
				ATLASSERT(::IsWindow(hNtfWnd));
				if(::IsWindow(hNtfWnd))
				{
					// need conditional code because types don't match in winuser.h
					#ifdef _WIN64
					::SetWindowLongPtr(hNtfWnd, GWLP_USERDATA, (LONG_PTR)this);
					#else
					::SetWindowLongPtr(hNtfWnd, GWLP_USERDATA, PtrToLong(this));
					#endif
					bRet = m_pSettingChangeNotify->Add(hNtfWnd);
				}
				else
				{
					bRet = FALSE;
				}
			}

			lock.Unlock();

			return bRet;
		}

		void TermSettingChangeNotify()
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::TermSettingChangeNotify.\n"));
				ATLASSERT(FALSE);
				return;
			}

			if(m_pSettingChangeNotify != NULL && m_pSettingChangeNotify->GetSize() > 0)
				::DestroyWindow((*m_pSettingChangeNotify)[0]);
			delete m_pSettingChangeNotify;
			m_pSettingChangeNotify = NULL;

			lock.Unlock();
		}

		BOOL AddSettingChangeNotify(HWND hWnd)
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::AddSettingChangeNotify.\n"));
				ATLASSERT(FALSE);
				return FALSE;
			}

			ATLASSERT(::IsWindow(hWnd));
			BOOL bRet = FALSE;
			if(InitSettingChangeNotify() != FALSE)
				bRet = m_pSettingChangeNotify->Add(hWnd);

			lock.Unlock();

			return bRet;
		}

		BOOL RemoveSettingChangeNotify(HWND hWnd)
		{
			CStaticDataInitCriticalSectionLock lock;
			if(FAILED(lock.Lock()))
			{
				ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::RemoveSettingChangeNotify.\n"));
				ATLASSERT(FALSE);
				return FALSE;
			}

			BOOL bRet = FALSE;
			if(m_pSettingChangeNotify != NULL)
				bRet = m_pSettingChangeNotify->Remove(hWnd);

			lock.Unlock();

			return bRet;
		}

		// Implementation - setting change notify dialog template and dialog procedure
		struct _ATL_EMPTY_DLGTEMPLATE : DLGTEMPLATE
		{
			_ATL_EMPTY_DLGTEMPLATE()
			{
				memset(this, 0, sizeof(_ATL_EMPTY_DLGTEMPLATE));
				style = WS_POPUP;
			}
			WORD wMenu, wClass, wTitle;
		};

		#ifdef _WIN64
		static INT_PTR CALLBACK _SettingChangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		#else
		static BOOL CALLBACK _SettingChangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		#endif
		{
			if(uMsg == WM_SETTINGCHANGE)
			{
				// need conditional code because types don't match in winuser.h
			#ifdef _WIN64
				CAppModule* pModule = (CAppModule*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			#else
				CAppModule* pModule = (CAppModule*)LongToPtr(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			#endif
				ATLASSERT(pModule != NULL);
				ATLASSERT(pModule->m_pSettingChangeNotify != NULL);
				const UINT uTimeout = 1500;   // ms
				for(int i = 1; i < pModule->m_pSettingChangeNotify->GetSize(); i++)
				{
					::SendMessageTimeout((*pModule->m_pSettingChangeNotify)[i], uMsg, wParam, lParam, SMTO_ABORTIFHUNG, uTimeout, NULL);
				}
				return TRUE;
			}
			return FALSE;
		}
	};


	///////////////////////////////////////////////////////////////////////////////
	// CServerAppModule - module class for a COM server application

	class CServerAppModule : public CAppModule
	{
	public:
		HANDLE m_hEventShutdown;
		bool m_bActivity;
		DWORD m_dwTimeOut;
		DWORD m_dwPause;

		// Override of CAppModule::Init
		HRESULT Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL)
		{
			m_dwTimeOut = 5000;
			m_dwPause = 1000;
			return CAppModule::Init(pObjMap, hInstance, pLibID);
		}

		void Term()
		{
			if(m_hEventShutdown != NULL && ::CloseHandle(m_hEventShutdown))
				m_hEventShutdown = NULL;
			CAppModule::Term();
		}

		// COM Server methods
		LONG Unlock()
		{
			LONG lRet = CComModule::Unlock();
			if(lRet == 0)
			{
				m_bActivity = true;
				::SetEvent(m_hEventShutdown); // tell monitor that we transitioned to zero
			}
			return lRet;
		}

		void MonitorShutdown()
		{
			for(;;)
			{
				::WaitForSingleObject(m_hEventShutdown, INFINITE);
				DWORD dwWait = 0;
				do
				{
					m_bActivity = false;
					dwWait = ::WaitForSingleObject(m_hEventShutdown, m_dwTimeOut);
				}
				while(dwWait == WAIT_OBJECT_0);
				// timed out
				if(!m_bActivity && m_nLockCnt == 0) // if no activity let's really bail
				{
					#if defined(_ATL_FREE_THREADED)
					::CoSuspendClassObjects();
					if(!m_bActivity && m_nLockCnt == 0)
					#endif
						break;
				}
			}
			// This handle should be valid now. If it isn't, 
			// check if _Module.Term was called first (it shouldn't)
			if(::CloseHandle(m_hEventShutdown))
				m_hEventShutdown = NULL;
			::PostThreadMessage(m_dwMainThreadID, WM_QUIT, 0, 0);
		}

		bool StartMonitor()
		{
			m_hEventShutdown = ::CreateEvent(NULL, false, false, NULL);
			if(m_hEventShutdown == NULL)
				return false;
			DWORD dwThreadID = 0;
			
			#if !defined(_ATL_MIN_CRT) && defined(_MT)
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (UINT (WINAPI*)(void*))MonitorProc, this, 0, (UINT*)&dwThreadID);
			#endif
		
			bool bRet = (hThread != NULL);
			if(bRet)
				::CloseHandle(hThread);
			return bRet;
		}

		static DWORD WINAPI MonitorProc(void* pv)
		{
			CServerAppModule* p = (CServerAppModule*)pv;
			p->MonitorShutdown();
			return 0;
		}
	};


	///////////////////////////////////////////////////////////////////////////////
	// CRegKeyEx - adds type-specific methods to CRegKey

	typedef ATL::CRegKey CRegKeyEx;


	///////////////////////////////////////////////////////////////////////////////
	// CString forward reference (enables CString use in asixuser.h and asixgdi.h)

	#if defined(_ASIX_FORWARD_DECLARE_CSTRING) && !defined(_ASIX_USE_CSTRING)
	#define _ASIX_USE_CSTRING
	#endif // defined(_ASIX_FORWARD_DECLARE_CSTRING) && !defined(_ASIX_USE_CSTRING)

	#ifdef _ASIX_USE_CSTRING
	class CString;   // forward declaration (include asixmisc.h for the whole class)
	#endif // _ASIX_USE_CSTRING

	// CString namespace
	#ifndef _CSTRING_NS
	#ifdef __ATLSTR_H__
	#define _CSTRING_NS	ATL
	#else
	#define _CSTRING_NS	Asix
	#endif
	#endif // _CSTRING_NS

	// Type classes namespace
	#ifndef _WTYPES_NS
	#ifdef __ATLTYPES_H__
	#define _WTYPES_NS
	#else
	#define _WTYPES_NS	Asix
	#endif
	#endif // _WTYPES_NS

}	// namespace Asix


///////////////////////////////////////////////////////////////////////////////
// General DLL version helpers
// ATL11: removed)

#if (_ATL_VER >= 0x0B00)

namespace ATL
{

	inline HRESULT AtlGetDllVersion(HINSTANCE hInstDLL, DLLVERSIONINFO* pDllVersionInfo)
	{
		ATLASSERT(pDllVersionInfo != NULL);
		if(pDllVersionInfo == NULL)
			return E_INVALIDARG;

		// We must get this function explicitly because some DLLs don't implement it.
		DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
		if(pfnDllGetVersion == NULL)
			return E_NOTIMPL;

		return (*pfnDllGetVersion)(pDllVersionInfo);
	}

	inline HRESULT AtlGetDllVersion(LPCTSTR lpstrDllName, DLLVERSIONINFO* pDllVersionInfo)
	{
		HINSTANCE hInstDLL = ::LoadLibrary(lpstrDllName);
		if(hInstDLL == NULL)
			return E_FAIL;
		HRESULT hRet = AtlGetDllVersion(hInstDLL, pDllVersionInfo);
		::FreeLibrary(hInstDLL);
		return hRet;
	}

	// Common Control Versions:
	//   Win95/WinNT 4.0    maj=4 min=00
	//   IE 3.x     maj=4 min=70
	//   IE 4.0     maj=4 min=71
	inline HRESULT AtlGetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
	{
		ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
		if(pdwMajor == NULL || pdwMinor == NULL)
			return E_INVALIDARG;

		DLLVERSIONINFO dvi;
		::ZeroMemory(&dvi, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);
		HRESULT hRet = AtlGetDllVersion(_T("comctl32.dll"), &dvi);

		if(SUCCEEDED(hRet))
		{
			*pdwMajor = dvi.dwMajorVersion;
			*pdwMinor = dvi.dwMinorVersion;
		}
		else if(hRet == E_NOTIMPL)
		{
			// If DllGetVersion is not there, then the DLL is a version
			// previous to the one shipped with IE 3.x
			*pdwMajor = 4;
			*pdwMinor = 0;
			hRet = S_OK;
		}

		return hRet;
	}

	// Shell Versions:
	//   WinXP                    maj=5 min=1
	
	inline HRESULT AtlGetShellVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
	{
		ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
		if(pdwMajor == NULL || pdwMinor == NULL)
			return E_INVALIDARG;

		DLLVERSIONINFO dvi;
		::ZeroMemory(&dvi, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);
		HRESULT hRet = AtlGetDllVersion(_T("shell32.dll"), &dvi);

		if(SUCCEEDED(hRet))
		{
			*pdwMajor = dvi.dwMajorVersion;
			*pdwMinor = dvi.dwMinorVersion;
		}
		else if(hRet == E_NOTIMPL)
		{
			// If DllGetVersion is not there, then the DLL is a version
			// previous to the one shipped with IE 4.x
			*pdwMajor = 5;
			*pdwMinor = 1;
			hRet = S_OK;
		}

		return hRet;
	}
}	// namespace ATL

#endif // (_ATL_VER < 0x0700) && defined(_ATL_DLL) && !defined(_WIN32_WCE)


// These are always included
#include "asixwinx.h"
#include "asixuser.h"
#include "asixgdi.h"

#ifndef _ASIX_NO_AUTOMATIC_NAMESPACE
//using namespace Asix;
#endif // !_ASIX_NO_AUTOMATIC_NAMESPACE

#endif // _ASIXAPP_INC_
