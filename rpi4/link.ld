MEMORY
{
  RAM(rwx) : ORIGIN = 0x0, LENGTH = 0x80000000 /* 2GiB */
}

SECTIONS
{
  . = ORIGIN(RAM);
  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
}
