#pragma once

#if defined(_MSC_VER)
#	pragma warning(push, 3)
#	pragma warning(disable : 4865)
#	pragma warning(disable : 5039)
#endif

#define _USE_MATH_DEFINES

#include <cstdint>
#include <cassert>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <string>
#include <string_view>
#include <span>
#include <vector>
#include <unordered_map>

#if defined(_WIN32)

#ifdef WINVER
#	undef WINVER
#endif
#	define WINVER 0x0601
#ifdef _WIN32_WINNT
#	undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0601

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NOCRYPT
#include <Windows.h>

#include <glad/gl.h>

#endif // _WIN32

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif