enum
{
  PERIPHERAL_BASE = 0xFE000000,
  AUX_BASE        = PERIPHERAL_BASE + 0x215000,
  AUX_MU_IO_REG   = AUX_BASE + 64,
  AUX_MU_LSR_REG  = AUX_BASE + 84,
};

typedef unsigned int uint32_t;
typedef long int64_t;

uint32_t mmioRead(int64_t reg)
{
  return *(volatile uint32_t*)reg;
}

void mmioWrite(int64_t reg, uint32_t val)
{
  *(volatile uint32_t*)reg = val;
}

uint32_t uartIsWriteCharReady()
{
  return mmioRead(AUX_MU_LSR_REG) & 0x20;
}

void uartWriteChar(char c)
{
  while (!uartIsWriteCharReady()); 
  mmioWrite(AUX_MU_IO_REG, c);
}

void uartWriteString(char *str)
{
  while (*str)
  {
    if (*str == '\n')
      uartWriteChar('\r');
    uartWriteChar(*str++);
  }
}

void print(char *type, uint32_t size)
{
  uartWriteString("sizeof ");
  uartWriteString(type);
  uartWriteString(": ");

  uartWriteChar('0' + size);
  uartWriteString("\n");
}

void uartWriteHello()
{
  uartWriteString("\nuartWriteHello:\n");

  char c;
  print("char", sizeof c);

  uint32_t u;
  print("uint32_t", sizeof u);

  int64_t i;
  print("int64_t", sizeof i);

  char* p;
  print("char*", sizeof p);
}
