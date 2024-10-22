#ifndef PARSER_UTIL_H_
#define PARSER_UTIL_H_
#include <string.h>
#include <math.h>

#define parser_min(a, b) (((a) < (b)) ? (a) : (b))
#define parser_max(a, b) (((a) > (b)) ? (a) : (b))

//=============buffer struct==========
typedef struct {
	unsigned char *data;
	unsigned int   length;
	unsigned int   readIdx;
	unsigned int   endIdx;
} streamBuffer;


#ifdef __cplusplus
extern "C"{
#endif

//=================parser function====
unsigned int bs_read_ue(unsigned char *pBuff, unsigned int nLen, unsigned int *nStartBit);

int bs_read_se(unsigned char *pBuff, unsigned int nLen, unsigned int *nStartBit);

unsigned int bs_read_u(unsigned int BitCount, unsigned char * buf, unsigned int *nStartBit);

unsigned int next_bits(unsigned int BitCount, unsigned char * buf, unsigned int *nStartBit);

//NAL起始码防竞争机制
void de_emulation_prevention(unsigned char* buf, unsigned int* buf_size);

//=================NALU function=====
/*func:00 00 01 起始字节
 *成功则返回：1*/
int FindStartCode2 (unsigned char *Buf);

/*func:00 00 00 01 起始字节
 *成功则返回：1*/
int FindStartCode3 (unsigned char *Buf);

/*func：从src中读取n个字节到dec中，指针向后移动n个字节
 *return：返回实际读取的字节数*/
int read_buffer(unsigned char *dec, unsigned int n, streamBuffer *src);

/*func:从src中读取1个字节并返回，指针向后移动1个字节*/
unsigned char get_char(streamBuffer *src);

/*func:移动src指针，与fseek()功能类似*/
int buffer_seek(unsigned int n, streamBuffer *src);

#ifdef __cplusplus
}
#endif


#endif /* PARSER_UTIL_H_ */
