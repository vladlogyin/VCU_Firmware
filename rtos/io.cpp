#include <rtos/io.h>

ostream::ostream(outputn stringcb, outputc charcb)
{
  this->_stringcb = stringcb;
  this->_charcb = charcb;
}

ostream& operator<<(ostream& os, const char* str)
{
  //os.buffer.sputn(str, strlen(str));
  os._stringcb(str,strlen(str));
  return os;
}

ostream& operator<<(ostream& os, long int num)
{
  if(num < 0)
  {
    return os << '-' << (long unsigned int)(-num);
  }
  else
  {
    return os << (long unsigned int)(num);
  }
}
ostream& operator<<(ostream& os, long unsigned int num)
{
  bool uppercase = true;
  char buf[11] = "\0\0\0\0\0\0\0\0\0\0";
  int n = 0;
  unsigned long long int d = 1;;
  int width = 1;

  while(num / d >= os.format.base || width < os.format.width)
  {
    d *= os.format.base;
    width++;
  }

  while(d != 0)
  {
    int dgt = num / d;
    num %= d;
    d /= os.format.base;

    if(n || (os.format.lz||dgt>0) || d == 0)
    {
      buf[n++] = dgt + (dgt < 10 ? '0' : (uppercase ? 'A' : 'a') - 10);
    }
  }

  return os << buf;
}
ostream& operator<<(ostream& os, const char c)
{
  os._charcb(c);
  return os;
}
ostream& operator<<(ostream& os, oformat::nbase f)
{
  os.format.base = f;
  return os;
}
ostream& operator<<(ostream& os, oformat of)
{
  os.format = of;
  return os;
}
