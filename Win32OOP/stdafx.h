// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente, pero
// son rara vez modificados
//

#pragma once
#define _WIN32_WINNT 0X0601
#include "targetver.h"

// Excluir material rara vez utilizado de encabezados de Windows


#ifdef UNICODE
#undef UNICODE
#endif

#ifdef _UNICODE
#undef _UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define NORPC
#define NOMCX
#define NOTAPE
#define NOKERNEL
#define NOSERVICE
#define NOKANJI
#define NOPROFILER
#define NOPROXYSTUP
//#define NOMINMAX
#define STRICT
// Archivos de encabezado de Windows
#include <windows.h>
#include <CommCtrl.h>
#include <commdlg.h>

#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "ComCtl32.lib")
// Archivos de encabezado en tiempo de ejecución de C
/*
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
*/

// hacer referencia aquí a los encabezados adicionales que el programa necesita