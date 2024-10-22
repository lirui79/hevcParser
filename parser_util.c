#include "parser_util.h"


unsigned int bs_read_ue(unsigned char *pBuff, unsigned int nLen, unsigned int *nStartBit)
{
	    //计算0bit的个数
		unsigned int nZeroNum = 0;
	    while (nStartBit[0] < nLen * 8)
	    {
	        if (pBuff[nStartBit[0] / 8] & (0x80 >> (nStartBit[0] % 8))) //&:按位与，%取余
	        {
	            break;
	        }
	        nZeroNum++;
	        nStartBit[0]++;
	    }
	    nStartBit[0]++;


	    //计算结果
	    unsigned int dwRet = 0;
	    for (unsigned int i=0; i<nZeroNum; i++)
	    {
	        dwRet <<= 1;
	        if (pBuff[nStartBit[0] / 8] & (0x80 >> (nStartBit[0] % 8)))
	        {
	            dwRet += 1;
	        }
	        nStartBit[0]++;
	    }
	    return (1 << nZeroNum) - 1 + dwRet;
}


int bs_read_se(unsigned char *pBuff, unsigned int nLen, unsigned int *nStartBit)
{
	    int UeVal = bs_read_ue(pBuff, nLen, nStartBit);
	    double k = UeVal;
	    int nValue = ceil(k/2);//ceil函数：ceil函数的作用是求不小于给定实数的最小整数。ceil(2)=ceil(1.2)=cei(1.5)=2.00
	    if (UeVal % 2==0)
	        nValue =- nValue;
	    return nValue;
}


unsigned int bs_read_u(unsigned int BitCount, unsigned char * buf, unsigned int *nStartBit)
{
		unsigned int dwRet = 0;
	    for (unsigned int i=0; i<BitCount; i++)
	    {
	        dwRet <<= 1;
	        if (buf[nStartBit[0] / 8] & (0x80 >> (nStartBit[0] % 8)))
	        {
	            dwRet += 1;
	        }
	        nStartBit[0]++;
	    }
	    return dwRet;
}

unsigned int next_bits(unsigned int BitCount, unsigned char * buf, unsigned int *nStartBit)
{
		unsigned int dwRet = 0;
		unsigned int startBit = nStartBit[0];
	   for (unsigned int i=0; i<BitCount; i++){
		   dwRet <<= 1;
			if (buf[nStartBit[0] / 8] & (0x80 >> (nStartBit[0] % 8))){
				dwRet += 1;
			}
			    nStartBit[0]++;
	    }
	   /*nStartBit回退到之前的位置，比特流的指针不移动*/
	   nStartBit[0] = startBit;
	   return dwRet;
}

	/**
	 * H264的NAL起始码防竞争机制
	 *
	 * @param buf SPS数据内容
	 *
	 * @无返回值
	 */
void de_emulation_prevention(unsigned char* buf,unsigned int* buf_size)
{
	    int i=0,j=0;
	    unsigned char* tmp_ptr=NULL;
	    unsigned int tmp_buf_size=0;
	    int val=0;

	    tmp_ptr=buf;
	    tmp_buf_size=*buf_size;
	    for(i=0;i<(tmp_buf_size-2);i++)
	    {
	        //check for 0x000003
	        val=(tmp_ptr[i]^0x00) +(tmp_ptr[i+1]^0x00)+(tmp_ptr[i+2]^0x03);
	        if(val==0)
	        {
	            //kick out 0x03
	            for(j=i+2;j<tmp_buf_size-1;j++)
	                tmp_ptr[j]=tmp_ptr[j+1];

	            //and so we should devrease bufsize
	            (*buf_size)--;
	        }
	    }
}

int FindStartCode2 (unsigned char *Buf){
	if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] !=1) return 0; //0x000001?
	else return 1;
}

int FindStartCode3 (unsigned char *Buf){
	if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] !=0 || Buf[3] !=1) return 0;//0x00000001?
	else return 1;
}

int read_buffer(unsigned char *dec, unsigned int n, streamBuffer *src){
	int ret = 0;
	if(src->length < n){
		ret = n - src->length;
	}
	else{
		ret = n;
	}
	memcpy(dec, src->data + src->readIdx, ret);
	src->length  = src->length - ret;
	src->readIdx = src->readIdx + ret;
	return ret;
}

unsigned char get_char(streamBuffer *src){
	unsigned char ret = src->data[src->readIdx];
	src->length  = src->length - 1;
	src->readIdx = src->readIdx + 1;
	return ret;
}

int buffer_seek(unsigned int n, streamBuffer *src){
	int ret = 0;
	int m = -n;
	if(src->length < m){
		ret = m - src->length;
		return 1;
	} else{
		src->length = src->length + m;
		src->readIdx = src->readIdx - m;
		return 0;
	}
}

