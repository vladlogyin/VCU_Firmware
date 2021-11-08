#ifndef IO_H
#define IO_H

#include <cstring>
#include <cstddef>
#include <sys/types.h>

#include <libopencm3/stm32/usart.h>


#include <lib/ringbuffer/ringbuffer.h>

#define RTOS_FILES_MAX 8
#define BUFFER_SIZE 64

class ibuffer : public basic_ringbuffer<64>
{

};

class obuffer : public basic_ringbuffer<64>
{

};

typedef size_t (*outputn) (const char*, size_t);
typedef void (*outputc) (const char);

typedef size_t (*inputn) (char*, size_t);
typedef char (*inputc) (void);


struct oformat {
public:
    enum nbase {base16=16, base10=10, base8=8, base2=2};
    bool lz:1;          /**<  Leading zeros */
    bool alt:1;         /**<  alternate form */
    bool uc:1;          /**<  Upper case (for base16 only) */
    unsigned int width=0; /**<  field width */
    unsigned int base=10;  /**<  number base (e.g.: 8, 10, 16) */

};


class ostream
{
public:
  ostream(outputn stringcb, outputc charcb);
  obuffer buffer;
  oformat format;

  friend ostream& operator<<(ostream& os, const char* str);
  friend ostream& operator<<(ostream& os, long int num);
  friend ostream& operator<<(ostream& os, long unsigned int num);
  friend ostream& operator<<(ostream& os, const char c);
  friend ostream& operator<<(ostream& os, oformat::nbase f);
  friend ostream& operator<<(ostream& os, oformat of);

  outputn _stringcb;
  outputc _charcb;
};



int usart_print(const char* str);

#endif // IO_H_INCLUDED
