// AsixUI Template Library - AsixUI version 1.0
// Copyright (C)  2013 Kusiv Inc.
// All rights reserved.


#ifndef _ASIXVER_INC_
#define _ASIXVER_INC_

#pragma once


///////////////////////////////////////////////////////////////////////////////
// Asix version number

#define _ASIX_VER	0x0100
//
// _WIN32_WINNT version constants, for XP Higher

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 						0x0501
#endif

#ifndef _RICHEDIT_VER
#define _RICHEDIT_VER						0x0200
#endif

#define _WIN32_WINNT_WINXP                  0x0501
#define _WIN32_WINNT_WS03                   0x0502
#define _WIN32_WINNT_WIN6                   0x0600
#define _WIN32_WINNT_VISTA                  0x0600
#define _WIN32_WINNT_WS08                   0x0600
#define _WIN32_WINNT_LONGHORN               0x0600
#define _WIN32_WINNT_WIN7                   0x0601
#define _WIN32_WINNT_WIN8                   0x0602

//
// _WIN32_IE_ version constants, for at least IE6
//

#define _WIN32_IE_IE60                      0x0600
#define _WIN32_IE_IE60SP1                   0x0601
#define _WIN32_IE_IE60SP2                   0x0603
#define _WIN32_IE_IE70                      0x0700
#define _WIN32_IE_IE80                      0x0800
#define _WIN32_IE_IE90                      0x0900
#define _WIN32_IE_IE100                     0x0A00

//
// IE <-> OS version mapping
//

#define _WIN32_IE_XP                        _WIN32_IE_IE60
#define _WIN32_IE_XPSP1                     _WIN32_IE_IE60SP1
#define _WIN32_IE_XPSP2                     _WIN32_IE_IE60SP2
#define _WIN32_IE_WS03                      0x0602
#define _WIN32_IE_WS03SP1                   _WIN32_IE_IE60SP2
#define _WIN32_IE_WIN6                      _WIN32_IE_IE70
#define _WIN32_IE_LONGHORN                  _WIN32_IE_IE70
#define _WIN32_IE_WIN7                      _WIN32_IE_IE80
#define _WIN32_IE_WIN8                      _WIN32_IE_IE100


//
// NTDDI version constants
//

#define NTDDI_WINXP                         0x05010000
#define NTDDI_WINXPSP1                      0x05010100
#define NTDDI_WINXPSP2                      0x05010200
#define NTDDI_WINXPSP3                      0x05010300
#define NTDDI_WINXPSP4                      0x05010400

#define NTDDI_WS03                          0x05020000
#define NTDDI_WS03SP1                       0x05020100
#define NTDDI_WS03SP2                       0x05020200
#define NTDDI_WS03SP3                       0x05020300
#define NTDDI_WS03SP4                       0x05020400

#define NTDDI_WIN6                          0x06000000
#define NTDDI_WIN6SP1                       0x06000100
#define NTDDI_WIN6SP2                       0x06000200
#define NTDDI_WIN6SP3                       0x06000300
#define NTDDI_WIN6SP4                       0x06000400

#define NTDDI_VISTA                         NTDDI_WIN6
#define NTDDI_VISTASP1                      NTDDI_WIN6SP1
#define NTDDI_VISTASP2                      NTDDI_WIN6SP2
#define NTDDI_VISTASP3                      NTDDI_WIN6SP3
#define NTDDI_VISTASP4                      NTDDI_WIN6SP4

#define NTDDI_LONGHORN  NTDDI_VISTA

#define NTDDI_WS08                          NTDDI_WIN6SP1
#define NTDDI_WS08SP2                       NTDDI_WIN6SP2
#define NTDDI_WS08SP3                       NTDDI_WIN6SP3
#define NTDDI_WS08SP4                       NTDDI_WIN6SP4

#define NTDDI_WIN7                          0x06010000
#define NTDDI_WIN8                          0x06020000

//
// masks for version macros
//
#define OSVERSION_MASK      0xFFFF0000
#define SPVERSION_MASK      0x0000FF00
#define SUBVERSION_MASK     0x000000FF


//
// macros to extract various version fields from the NTDDI version
//
#define OSVER(Version)  ((Version) & OSVERSION_MASK)
#define SPVER(Version)  (((Version) & SPVERSION_MASK) >> 8)
#define SUBVER(Version) (((Version) & SUBVERSION_MASK) )


#if defined(DECLSPEC_DEPRECATED_DDK)

// deprecate in 2k or later
#if (NTDDI_VERSION >= NTDDI_WIN2K)
#define DECLSPEC_DEPRECATED_DDK_WIN2K DECLSPEC_DEPRECATED_DDK
#else
#define DECLSPEC_DEPRECATED_DDK_WIN2K
#endif

// deprecate in XP or later
#if (NTDDI_VERSION >= NTDDI_WINXP)
#define DECLSPEC_DEPRECATED_DDK_WINXP DECLSPEC_DEPRECATED_DDK
#else
#define DECLSPEC_DEPRECATED_DDK_WINXP
#endif

// deprecate in WS03 or later
#if (NTDDI_VERSION >= NTDDI_WS03)
#define DECLSPEC_DEPRECATED_DDK_WIN2003 DECLSPEC_DEPRECATED_DDK
#else
#define DECLSPEC_DEPRECATED_DDK_WIN2003
#endif

// deprecate in WIN6 or later
#if (NTDDI_VERSION >= NTDDI_WIN6)
#define DECLSPEC_DEPRECATED_DDK_WIN6 DECLSPEC_DEPRECATED_DDK
#else
#define DECLSPEC_DEPRECATED_DDK_WIN6
#endif

#define DECLSPEC_DEPRECATED_DDK_LONGHORN DECLSPEC_DEPRECATED_DDK_WIN6

#endif // defined(DECLSPEC_DEPRECATED_DDK)


//
// if versions aren't already defined, default to most current
//

#define NTDDI_VERSION_FROM_WIN32_WINNT2(ver)    ver##0000
#define NTDDI_VERSION_FROM_WIN32_WINNT(ver)     NTDDI_VERSION_FROM_WIN32_WINNT2(ver)

#if !defined(_WIN32_WINNT) && !defined(_CHICAGO_)
#define  _WIN32_WINNT   0x0601
#endif

#ifndef NTDDI_VERSION
#ifdef _WIN32_WINNT
// set NTDDI_VERSION based on _WIN32_WINNT
#define NTDDI_VERSION   NTDDI_VERSION_FROM_WIN32_WINNT(_WIN32_WINNT)
#else
#define NTDDI_VERSION   0x06020000
#endif
#endif

#ifndef WINVER
#ifdef _WIN32_WINNT
// set WINVER based on _WIN32_WINNT
#define WINVER          _WIN32_WINNT
#else
#define WINVER          0x0602
#endif
#endif

#ifndef _WIN32_IE
#ifdef _WIN32_WINNT
// set _WIN32_IE based on _WIN32_WINNT
#if (_WIN32_WINNT < _WIN32_WINNT_WINXP)
#error AsixUI need at least Windows XP
#elif (_WIN32_WINNT == _WIN32_WINNT_WINXP)
#define _WIN32_IE       _WIN32_IE_IE60
#elif (_WIN32_WINNT <= _WIN32_WINNT_WS03)
#define _WIN32_IE       _WIN32_IE_WS03
#elif (_WIN32_WINNT <= _WIN32_WINNT_VISTA)
#define _WIN32_IE       _WIN32_IE_LONGHORN
#elif (_WIN32_WINNT <= _WIN32_WINNT_WIN7)
#define _WIN32_IE       _WIN32_IE_WIN7
#elif (_WIN32_WINNT <= _WIN32_WINNT_WIN8)
#define _WIN32_IE       _WIN32_IE_WIN8
#else
#define _WIN32_IE       0x0A00
#endif
#else
#define _WIN32_IE       0x0A00
#endif
#endif

//
// Sanity check for compatible versions
//
#if defined(_WIN32_WINNT) && !defined(MIDL_PASS) && !defined(RC_INVOKED)


#if (((OSVERSION_MASK & NTDDI_VERSION) == NTDDI_WINXP) && (_WIN32_WINNT != _WIN32_WINNT_WINXP))
#error NTDDI_VERSION setting conflicts with _WIN32_WINNT setting
#endif

#if (((OSVERSION_MASK & NTDDI_VERSION) == NTDDI_WS03) && (_WIN32_WINNT != _WIN32_WINNT_WS03))
#error NTDDI_VERSION setting conflicts with _WIN32_WINNT setting
#endif

#if (((OSVERSION_MASK & NTDDI_VERSION) == NTDDI_VISTA) && (_WIN32_WINNT != _WIN32_WINNT_VISTA))
#error NTDDI_VERSION setting conflicts with _WIN32_WINNT setting
#endif

#endif  // defined(_WIN32_WINNT) && !defined(MIDL_PASS) && !defined(_WINRESRC_)

#define WIN32_LEAN_AND_MEAN //  浠?Windows 澶存枃浠朵腑鎺掗櫎鏋佸皯浣跨敤鐨勪俊鎭?

#endif  // !ASIXVER_INC_

