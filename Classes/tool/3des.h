#pragma once

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef _uint32
#define _uint32 unsigned long int
#endif
typedef struct
{
	_uint32 esk[32];     /* DES encryption subkeys */
	_uint32 dsk[32];     /* DES decryption subkeys */
}des_context;
typedef struct
{
	_uint32 esk[96];     /* Triple-DES encryption subkeys */
	_uint32 dsk[96];     /* Triple-DES decryption subkeys */
}des3_context;
#include <stdio.h>
#include <string>
namespace util
{
	class DES_3
	{
	public:
		static	std::string  des_base64_encrypt(const char* text,const char* strKey,char addChar='\0')
		{
			des_context  ctx;
			des3_context ctx3;
			uint8 key[24];
			std::string strIn=text;
			//设置key
			size_t keyLength=strlen(strKey);
			memcpy(key,strKey,keyLength);
			if(keyLength==8)
			{
				des_set_key( &ctx, key );
			}
			else if(keyLength==16)
			{
				des3_set_2keys(&ctx3,key,key+8);
			}
			else if(keyLength==24)
			{
				des3_set_3keys(&ctx3,key,key+8,key+16);
			}
			else
			{
				return std::string();
			}
			//strIn不是8的倍数就补齐
			int addNum=strIn.length()%8;  
			//if(addNum!=0)
			//{
			  addNum=8-addNum;
			  while(addNum-- >=0)strIn.push_back(addChar);
			//}
			//开始8位8位的加密
			int circleNum=int(strIn.length()/8);    
			uint8 buf[8];
			int    inbaseNum=circleNum*8; 
			uint8* inBase=new uint8[inbaseNum];
			int    outbaseNum=(inbaseNum+2)/3*4+1;
			char*  outbase=new char[outbaseNum];  
			std::string outResult;
			if(inBase!=NULL && outbase!=NULL)
			{
				memset(inBase,0,inbaseNum);
				memset(outbase,0,outbaseNum);
				for (int i=0;i<circleNum;i++)
				{
					memcpy(buf,strIn.c_str()+i*8,8);
					if(keyLength==8)
					{
						des_encrypt( &ctx, buf,buf);
					}
					else 
					{
                        des3_encrypt(&ctx3,buf,buf);
					}
						
					memcpy(inBase+i*8,buf,8);
				}
				//des3加密完了之后再用base_64加密
				base64_encode(inBase,inbaseNum,outbase,outbaseNum);
				outbase[outbaseNum-1]='\0';
				outResult=outbase;            
				delete []inBase;
				delete []outbase;
			}
			return outResult;
		}



		static	std::string  base64_des_decrypt(const char* text,const char* strKey)
		{
			des_context  ctx;
			des3_context ctx3;
			uint8 key[24];
			std::string strIn=text;
			//设置key
			size_t keyLength=strlen(strKey);
			memcpy(key,strKey,keyLength);
			if(keyLength==8)
			{
				des_set_key( &ctx, key );
			}
			else if(keyLength==16)
			{
				des3_set_2keys(&ctx3,key,key+8);
			}
			else if(keyLength==24)
			{
				des3_set_3keys(&ctx3,key,key+8,key+16);
			}
			else
			{
				return std::string();
			}
			//先整体base64解密
			int textLength=int(strIn.length());
			//确保是4的倍数
			if(textLength%4!=0)
			{
		       return std::string();
			}
			int     outbaseNum=textLength/4*3+1;     
			uint8*  outbase=new uint8[outbaseNum];
			if(outbase==NULL)return std::string();
			memset(outbase,0,outbaseNum);
			base64_decode(strIn.c_str(),textLength,outbase,outbaseNum);
			//开始8位8位的解密
			int circleNum=int (outbaseNum/8);
			uint8 buf[8];
			std::string outResult;
			int    outDesNum=circleNum*8+1;
			char * outDes=new char[outDesNum];
			if(outDes==NULL)return std::string();
			memset(outDes,0,outDesNum);
				for (int i=0;i<circleNum;i++)
				{
					memcpy(buf,outbase+i*8,8);
					if(keyLength==8)
					{
						des_decrypt( &ctx, buf,buf);
					}
					else 
					{
						des3_decrypt(&ctx3,buf,buf);
					}

					memcpy(outDes+i*8,buf,8);
				}
				outResult=outDes;
				delete []outbase;
				delete []outDes;
			
			return outResult;
		}

	private:
		/* the eight DES S-boxes */
		const static _uint32 SB1[64];
		const static _uint32 SB2[64];
		const static _uint32 SB3[64];
		const static _uint32 SB4[64];
		const static _uint32 SB5[64];
		const static _uint32 SB6[64];
		const static _uint32 SB7[64];
		const static _uint32 SB8[64];
		/* PC1: left and right halves bit-swap */
		const static _uint32 LHs[16];
		const static _uint32 RHs[16];
		/* platform-independant 32-bit integer manipulation macros */
#define GET_UINT32(n,b,i)                       \
		{                                               \
		(n) = ( (_uint32) (b)[(i)    ] << 24 )       \
		| ( (_uint32) (b)[(i) + 1] << 16 )       \
		| ( (_uint32) (b)[(i) + 2] <<  8 )       \
		| ( (_uint32) (b)[(i) + 3]       );      \
		}
#define PUT_UINT32(n,b,i)                       \
		{                                               \
		(b)[(i)    ] = (uint8) ( (n) >> 24 );       \
		(b)[(i) + 1] = (uint8) ( (n) >> 16 );       \
		(b)[(i) + 2] = (uint8) ( (n) >>  8 );       \
		(b)[(i) + 3] = (uint8) ( (n)       );       \
		}
		/* Initial Permutation macro */
#define DES_IP(X,Y)                                             \
		{                                                               \
		T = ((X >>  4) ^ Y) & 0x0F0F0F0F; Y ^= T; X ^= (T <<  4);   \
		T = ((X >> 16) ^ Y) & 0x0000FFFF; Y ^= T; X ^= (T << 16);   \
		T = ((Y >>  2) ^ X) & 0x33333333; X ^= T; Y ^= (T <<  2);   \
		T = ((Y >>  8) ^ X) & 0x00FF00FF; X ^= T; Y ^= (T <<  8);   \
		Y = ((Y << 1) | (Y >> 31)) & 0xFFFFFFFF;                    \
		T = (X ^ Y) & 0xAAAAAAAA; Y ^= T; X ^= T;                   \
		X = ((X << 1) | (X >> 31)) & 0xFFFFFFFF;                    \
		}

		/* Final Permutation macro */

#define DES_FP(X,Y)                                             \
		{                                                               \
		X = ((X << 31) | (X >> 1)) & 0xFFFFFFFF;                    \
		T = (X ^ Y) & 0xAAAAAAAA; X ^= T; Y ^= T;                   \
		Y = ((Y << 31) | (Y >> 1)) & 0xFFFFFFFF;                    \
		T = ((Y >>  8) ^ X) & 0x00FF00FF; X ^= T; Y ^= (T <<  8);   \
		T = ((Y >>  2) ^ X) & 0x33333333; X ^= T; Y ^= (T <<  2);   \
		T = ((X >> 16) ^ Y) & 0x0000FFFF; Y ^= T; X ^= (T << 16);   \
		T = ((X >>  4) ^ Y) & 0x0F0F0F0F; Y ^= T; X ^= (T <<  4);   \
		}

		/* DES round macro */

#define DES_ROUND(X,Y)                          \
		{                                               \
		T = *SK++ ^ X;                              \
		Y ^= SB8[ (T      ) & 0x3F ] ^              \
		SB6[ (T >>  8) & 0x3F ] ^              \
		SB4[ (T >> 16) & 0x3F ] ^              \
		SB2[ (T >> 24) & 0x3F ];               \
		\
		T = *SK++ ^ ((X << 28) | (X >> 4));         \
		Y ^= SB7[ (T      ) & 0x3F ] ^              \
		SB5[ (T >>  8) & 0x3F ] ^              \
		SB3[ (T >> 16) & 0x3F ] ^              \
		SB1[ (T >> 24) & 0x3F ];               \
		}
		/* DES key schedule */

		static int des_main_ks( _uint32 SK[32], uint8 key[8] )
		{
			int i;
			_uint32 X, Y, T;

			GET_UINT32( X, key, 0 );
			GET_UINT32( Y, key, 4 );

			/* Permuted Choice 1 */

			T =  ((Y >>  4) ^ X) & 0x0F0F0F0F;  X ^= T; Y ^= (T <<  4);
			T =  ((Y      ) ^ X) & 0x10101010;  X ^= T; Y ^= (T      );

			X =   (LHs[ (X      ) & 0xF] << 3) | (LHs[ (X >>  8) & 0xF ] << 2)
				| (LHs[ (X >> 16) & 0xF] << 1) | (LHs[ (X >> 24) & 0xF ]     )
				| (LHs[ (X >>  5) & 0xF] << 7) | (LHs[ (X >> 13) & 0xF ] << 6)
				| (LHs[ (X >> 21) & 0xF] << 5) | (LHs[ (X >> 29) & 0xF ] << 4);

			Y =   (RHs[ (Y >>  1) & 0xF] << 3) | (RHs[ (Y >>  9) & 0xF ] << 2)
				| (RHs[ (Y >> 17) & 0xF] << 1) | (RHs[ (Y >> 25) & 0xF ]     )
				| (RHs[ (Y >>  4) & 0xF] << 7) | (RHs[ (Y >> 12) & 0xF ] << 6)
				| (RHs[ (Y >> 20) & 0xF] << 5) | (RHs[ (Y >> 28) & 0xF ] << 4);

			X &= 0x0FFFFFFF;
			Y &= 0x0FFFFFFF;

			/* calculate subkeys */

			for( i = 0; i < 16; i++ )
			{
				if( i < 2 || i == 8 || i == 15 )
				{
					X = ((X <<  1) | (X >> 27)) & 0x0FFFFFFF;
					Y = ((Y <<  1) | (Y >> 27)) & 0x0FFFFFFF;
				}
				else
				{
					X = ((X <<  2) | (X >> 26)) & 0x0FFFFFFF;
					Y = ((Y <<  2) | (Y >> 26)) & 0x0FFFFFFF;
				}

				*SK++ =   ((X <<  4) & 0x24000000) | ((X << 28) & 0x10000000)
					| ((X << 14) & 0x08000000) | ((X << 18) & 0x02080000)
					| ((X <<  6) & 0x01000000) | ((X <<  9) & 0x00200000)
					| ((X >>  1) & 0x00100000) | ((X << 10) & 0x00040000)
					| ((X <<  2) & 0x00020000) | ((X >> 10) & 0x00010000)
					| ((Y >> 13) & 0x00002000) | ((Y >>  4) & 0x00001000)
					| ((Y <<  6) & 0x00000800) | ((Y >>  1) & 0x00000400)
					| ((Y >> 14) & 0x00000200) | ((Y      ) & 0x00000100)
					| ((Y >>  5) & 0x00000020) | ((Y >> 10) & 0x00000010)
					| ((Y >>  3) & 0x00000008) | ((Y >> 18) & 0x00000004)
					| ((Y >> 26) & 0x00000002) | ((Y >> 24) & 0x00000001);

				*SK++ =   ((X << 15) & 0x20000000) | ((X << 17) & 0x10000000)
					| ((X << 10) & 0x08000000) | ((X << 22) & 0x04000000)
					| ((X >>  2) & 0x02000000) | ((X <<  1) & 0x01000000)
					| ((X << 16) & 0x00200000) | ((X << 11) & 0x00100000)
					| ((X <<  3) & 0x00080000) | ((X >>  6) & 0x00040000)
					| ((X << 15) & 0x00020000) | ((X >>  4) & 0x00010000)
					| ((Y >>  2) & 0x00002000) | ((Y <<  8) & 0x00001000)
					| ((Y >> 14) & 0x00000808) | ((Y >>  9) & 0x00000400)
					| ((Y      ) & 0x00000200) | ((Y <<  7) & 0x00000100)
					| ((Y >>  7) & 0x00000020) | ((Y >>  3) & 0x00000011)
					| ((Y <<  2) & 0x00000004) | ((Y >> 21) & 0x00000002);
			}

			return( 0 );
		}

		static int des_set_key( des_context *ctx, uint8 key[8] )
		{
			int i;

			/* setup encryption subkeys */

			des_main_ks( ctx->esk, key );

			/* setup decryption subkeys */

			for( i = 0; i < 32; i += 2 )
			{
				ctx->dsk[i    ] = ctx->esk[30 - i];
				ctx->dsk[i + 1] = ctx->esk[31 - i];
			}

			return( 0 );
		}

		/* DES 64-bit block encryption/decryption */

		static void des_crypt( _uint32 SK[32], uint8 input[8], uint8 output[8] )
		{
			_uint32 X, Y, T;

			GET_UINT32( X, input, 0 );
			GET_UINT32( Y, input, 4 );

			DES_IP( X, Y );

			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );

			DES_FP( Y, X );

			PUT_UINT32( Y, output, 0 );
			PUT_UINT32( X, output, 4 );
		}

		static void des_encrypt( des_context *ctx, uint8 input[8], uint8 output[8] )
		{
			des_crypt( ctx->esk, input, output );
		}

		static void des_decrypt( des_context *ctx, uint8 input[8], uint8 output[8] )
		{
			des_crypt( ctx->dsk, input, output );
		}

		/* Triple-DES key schedule */

		static int des3_set_2keys( des3_context *ctx, uint8 key1[8], uint8 key2[8] )
		{
			int i;

			des_main_ks( ctx->esk     , key1 );
			des_main_ks( ctx->dsk + 32, key2 );

			for( i = 0; i < 32; i += 2 )
			{
				ctx->dsk[i     ] = ctx->esk[30 - i];
				ctx->dsk[i +  1] = ctx->esk[31 - i];

				ctx->esk[i + 32] = ctx->dsk[62 - i];
				ctx->esk[i + 33] = ctx->dsk[63 - i];

				ctx->esk[i + 64] = ctx->esk[     i];
				ctx->esk[i + 65] = ctx->esk[ 1 + i];

				ctx->dsk[i + 64] = ctx->dsk[     i];
				ctx->dsk[i + 65] = ctx->dsk[ 1 + i];
			}

			return( 0 );
		}

		static int des3_set_3keys( des3_context *ctx, uint8 key1[8], uint8 key2[8],
			uint8 key3[8] )
		{
			int i;

			des_main_ks( ctx->esk     , key1 );
			des_main_ks( ctx->dsk + 32, key2 );
			des_main_ks( ctx->esk + 64, key3 );

			for( i = 0; i < 32; i += 2 )
			{
				ctx->dsk[i     ] = ctx->esk[94 - i];
				ctx->dsk[i +  1] = ctx->esk[95 - i];

				ctx->esk[i + 32] = ctx->dsk[62 - i];
				ctx->esk[i + 33] = ctx->dsk[63 - i];

				ctx->dsk[i + 64] = ctx->esk[30 - i];
				ctx->dsk[i + 65] = ctx->esk[31 - i];
			}

			return( 0 );
		}

		/* Triple-DES 64-bit block encryption/decryption */

		static void des3_crypt( _uint32 SK[96], uint8 input[8], uint8 output[8] )
		{
			_uint32 X, Y, T;

			GET_UINT32( X, input, 0 );
			GET_UINT32( Y, input, 4 );

			DES_IP( X, Y );

			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );

			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );
			DES_ROUND( X, Y );  DES_ROUND( Y, X );

			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );
			DES_ROUND( Y, X );  DES_ROUND( X, Y );

			DES_FP( Y, X );

			PUT_UINT32( Y, output, 0 );
			PUT_UINT32( X, output, 4 );
		}

		static void des3_encrypt( des3_context *ctx, uint8 input[8], uint8 output[8] )
		{
			des3_crypt( ctx->esk, input, output );
		}

		static void des3_decrypt( des3_context *ctx, uint8 input[8], uint8 output[8] )
		{
			des3_crypt( ctx->dsk, input, output );
		}

public:
		static int base64_encode(const uint8 *inStr,int inLen,char *outStr,int outSize)
		{
			static std::string strCode = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			const char *baseCode = strCode.c_str();
			if(inStr == NULL || outStr == NULL || inLen * 4/3 >outSize) return -1;
			int outLen=0;
			unsigned long value;
			int i=0;
			int tmpLen = inLen%3;
			for(i=0;i<inLen-tmpLen;i+=3)
			{

				value = (inStr[i]<<16)+(inStr[i+1]<<8)+inStr[i+2];

				outStr[outLen] = baseCode[(value>>18)];
				outStr[outLen+1] = baseCode[((value & 0x0003F000)>>12)];
				outStr[outLen+2] = baseCode[((value & 0x00000FC0)>>6)];
				outStr[outLen+3] = baseCode[(value & 0x0000003F)];
				outLen+=4;
			}

			if(tmpLen > 0)
			{
				if(tmpLen == 1)
				{
					value = (inStr[i]<<16);
					outStr[outLen] = baseCode[(value>>18)];
					outStr[outLen+1] = baseCode[((value & 0x0003F000)>>12)];
					outLen+=2;
				}
				else if(tmpLen == 2)
				{
					value = (inStr[i]<<16)+(inStr[i+1]<<8);
					outStr[outLen] = baseCode[(value>>18)];
					outStr[outLen+1] = baseCode[((value & 0x0003F000)>>12)];
					outStr[outLen+2] = baseCode[((value & 0x00000FC0)>>6)];
					outLen+=3;
				}
				for(i=0;i<3-tmpLen;i++)
				{
					outStr[outLen] = '=';
					outLen++;
				}
			}
			return outLen;
		}

		static int get_base_code(char value)
		{
			if(value >= 'A' && value <='Z'){
				return value - 'A';
			}
			else if(value >='a' && value <='z'){
				return value - 'a'+26;
			}
			else if(value  >='0' && value <='9'){
				return value -'0'+52;
			}
			else if(value == '+')
			{
				return 62;
			}
			else if(value == '/')
			{
				return 63;
			}
			else{
				return 0;
			}
		}
		static int base64_decode(const char *inStr,int inLen,uint8 *outStr,int outSize)
		{
			if(inStr == NULL || outStr == NULL || inLen > outSize * 4/3) return -1;
			int tmpLen = 0;
			int outLen = 0;
			int value = 0;
			int i=0;
			//char cValue[4];
			if(inLen >=4)
			{
				tmpLen =  inLen%4;
				if(tmpLen <= 2)
				{
					if(tmpLen == 1)
					{
						if(inStr[inLen-1] != '=') return -2;
					}
					else if(tmpLen == 2){
						if(!(inStr[inLen-2] == '=' && inStr[inLen-1] == '=')) return -3;
					}
				}
				else{
					return -4;
				}
				//TWFu  -- T:19,W:22,F:5,u:46
				for(i=0;i<inLen-tmpLen;i+=4)
				{
					value = (get_base_code(inStr[i])<<18) + (get_base_code(inStr[i+1])<<12) + (get_base_code(inStr[i+2])<<6) + (get_base_code(inStr[i+3]));
					//cValue[0] = get_base_code(inStr[i]);
					//cValue[1] = get_base_code(inStr[i+1]);
					//cValue[2] = get_base_code(inStr[i+2]);
					//cValue[3] = get_base_code(inStr[i+3]);
					//value = (cValue[0]<<18)+(cValue[1]<<12)+(cValue[2]<<6)+(cValue[3]);
					outStr[outLen] = ((value & 0x00FF0000)>>16);
					outStr[outLen+1] = ((value & 0x0000FF00)>>8);
					outStr[outLen+2] = (value & 0x000000FF);
					outLen+=3;
				}
				outLen -= tmpLen;
			}
			return outLen;
		}

	};

}

