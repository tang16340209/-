/**
 * 中山大学数据科学与计算机学院  16级软件工程电子政务方向 
 * author：唐育涛 16340209 
 * md5码的设计实现 
 */
#include <iostream>
#include <cstring> 
#include "md5.h"
using namespace std;

/* 初始化消息填充  第一位为1 其余为0*/
const byte MD5::PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};
/*十六进制与字符之前的对应关系*/ 
const char MD5::HEX_NUMBERS[16] = {
  	'0', '1', '2', '3',
  	'4', '5', '6', '7',
  	'8', '9', 'a', 'b',
  	'c', 'd', 'e', 'f'
};

/**
 * message是传输的消息，将被转化成消息散列值 
 */
MD5::MD5(const string& message) {
  	finished = false;
  
  	count[0] = count[1] = 0;
  	/* 初始化ABCD的值，采用小端存储 */
  	state[0] = 0x67452301;
  	state[1] = 0xefcdab89;
  	state[2] = 0x98badcfe;
  	state[3] = 0x10325476;

  	/* 根据message初始化 */
  	init((const byte*)message.c_str(), message.length());
}

/**
 * 返回MD结果 
 */
const byte* MD5::getDigest() {
  	if (!finished) {
    	finished = true;

    	byte bits[8];
    	bit32 oldState[4];
    	bit32 oldCount[2];
    	bit32 index, padLen;

    	/* 保存当前的A,B,C,D和长度计数 */
    	memcpy(oldState, state, 16);
    	memcpy(oldCount, count, 8);

    	/* 长度换成64位的二进制整数表示*/
    	encode(count, bits, 8);

    	/*添加原始消息长度到填充后的消息后面*/
    	index = (bit32)((count[0] >> 3) & 0x3f);
    	padLen = (index < 56) ? (56 - index) : (120 - index);
    	init(PADDING, padLen);

    	/* 填充长度 */
    	init(bits, 8);

    	/* 甲A，B,C,D级联得到结果*/
    	encode(state, digest, 16);

    	/* Restore current state and count. */
    	memcpy(state, oldState, 16);
    	memcpy(count, oldCount, 8);
  	}
  	return digest;
}

/**
 * 信息初始化函数 
 */
void MD5::init(const byte* input, size_t len) {

	bit32 i, index, partLen;

  	finished = false;
	//模64的结果余数    
  	index = (bit32)((count[0] >> 3) & 0x3f);
	/**
	 *  分这里是计算长度 
	 */
  	if ((count[0] += ((bit32)len << 3)) < ((bit32)len << 3)) {
    	++count[1];
  	}
  	count[1] += ((bit32)len >> 29);
	
  	partLen = 64 - index;

  	if (len >= partLen) {

    	memcpy(&buffer[index], input, partLen);
    	transform(buffer);

    	for (i = partLen; i + 63 < len; i += 64) {
      		transform(&input[i]);
    	}
    	index = 0;
  	} else {
    	i = 0;
  	}
  memcpy(&buffer[index], &input[i], len - i);
}

/**
 *  开始 4轮迭代，每轮16步，共64步 
 */
void MD5::transform(const byte block[64]) {


  	bit32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	
	/**
	 * 姜传进来的字节换成二进制表示，并转化为16个bits分组 
	 */
  	decode(block, x, 64);
	
	
	/*
	 * @ 算法核心部分 4轮迭代 
	 */
	
  	/* 1 轮 迭代*/
  	FF (a, b, c, d, x[ 0], rolarray[0][0], 0xd76aa478);
  	FF (d, a, b, c, x[ 1], rolarray[0][1], 0xe8c7b756);
  	FF (c, d, a, b, x[ 2], rolarray[0][2], 0x242070db);
  	FF (b, c, d, a, x[ 3], rolarray[0][3], 0xc1bdceee);
  	FF (a, b, c, d, x[ 4], rolarray[0][0], 0xf57c0faf);
  	FF (d, a, b, c, x[ 5], rolarray[0][1], 0x4787c62a);
  	FF (c, d, a, b, x[ 6], rolarray[0][2], 0xa8304613);
  	FF (b, c, d, a, x[ 7], rolarray[0][3], 0xfd469501);
  	FF (a, b, c, d, x[ 8], rolarray[0][0], 0x698098d8);
  	FF (d, a, b, c, x[ 9], rolarray[0][1], 0x8b44f7af);
  	FF (c, d, a, b, x[10], rolarray[0][2], 0xffff5bb1);
  	FF (b, c, d, a, x[11], rolarray[0][3], 0x895cd7be);
  	FF (a, b, c, d, x[12], rolarray[0][0], 0x6b901122);
  	FF (d, a, b, c, x[13], rolarray[0][1], 0xfd987193);
  	FF (c, d, a, b, x[14], rolarray[0][2], 0xa679438e);
  	FF (b, c, d, a, x[15], rolarray[0][3], 0x49b40821);

  	/* 2 轮 迭代*/
  	GG (a, b, c, d, x[ 1], rolarray[1][0], 0xf61e2562);
  	GG (d, a, b, c, x[ 6], rolarray[1][1], 0xc040b340);
  	GG (c, d, a, b, x[11], rolarray[1][2], 0x265e5a51);
  	GG (b, c, d, a, x[ 0], rolarray[1][3], 0xe9b6c7aa);
  	GG (a, b, c, d, x[ 5], rolarray[1][0], 0xd62f105d);
  	GG (d, a, b, c, x[10], rolarray[1][1],  0x2441453);
  	GG (c, d, a, b, x[15], rolarray[1][2], 0xd8a1e681);
  	GG (b, c, d, a, x[ 4], rolarray[1][3], 0xe7d3fbc8);
  	GG (a, b, c, d, x[ 9], rolarray[1][0], 0x21e1cde6);
  	GG (d, a, b, c, x[14], rolarray[1][1], 0xc33707d6);
  	GG (c, d, a, b, x[ 3], rolarray[1][2], 0xf4d50d87);
  	GG (b, c, d, a, x[ 8], rolarray[1][3], 0x455a14ed);
  	GG (a, b, c, d, x[13], rolarray[1][0], 0xa9e3e905);
  	GG (d, a, b, c, x[ 2], rolarray[1][1], 0xfcefa3f8);
  	GG (c, d, a, b, x[ 7], rolarray[1][2], 0x676f02d9);
  	GG (b, c, d, a, x[12], rolarray[1][3], 0x8d2a4c8a);

    /* 3 轮 迭代*/
  	HH (a, b, c, d, x[ 5], rolarray[2][0], 0xfffa3942);
  	HH (d, a, b, c, x[ 8], rolarray[2][1], 0x8771f681);
  	HH (c, d, a, b, x[11], rolarray[2][2], 0x6d9d6122);
  	HH (b, c, d, a, x[14], rolarray[2][3], 0xfde5380c);
  	HH (a, b, c, d, x[ 1], rolarray[2][0], 0xa4beea44);
  	HH (d, a, b, c, x[ 4], rolarray[2][1], 0x4bdecfa9);
  	HH (c, d, a, b, x[ 7], rolarray[2][2], 0xf6bb4b60);
  	HH (b, c, d, a, x[10], rolarray[2][3], 0xbebfbc70);
  	HH (a, b, c, d, x[13], rolarray[2][0], 0x289b7ec6);
  	HH (d, a, b, c, x[ 0], rolarray[2][1], 0xeaa127fa);
  	HH (c, d, a, b, x[ 3], rolarray[2][2], 0xd4ef3085);
  	HH (b, c, d, a, x[ 6], rolarray[2][3],  0x4881d05);
  	HH (a, b, c, d, x[ 9], rolarray[2][0], 0xd9d4d039);
  	HH (d, a, b, c, x[12], rolarray[2][1], 0xe6db99e5);
  	HH (c, d, a, b, x[15], rolarray[2][2], 0x1fa27cf8);
  	HH (b, c, d, a, x[ 2], rolarray[2][3], 0xc4ac5665);

  	/* 4 轮 迭代*/
  	II (a, b, c, d, x[ 0], rolarray[3][0], 0xf4292244);
  	II (d, a, b, c, x[ 7], rolarray[3][1], 0x432aff97);
  	II (c, d, a, b, x[14], rolarray[3][2], 0xab9423a7);
  	II (b, c, d, a, x[ 5], rolarray[3][3], 0xfc93a039);
  	II (a, b, c, d, x[12], rolarray[3][0], 0x655b59c3);
  	II (d, a, b, c, x[ 3], rolarray[3][1], 0x8f0ccc92);
  	II (c, d, a, b, x[10], rolarray[3][2], 0xffeff47d);
  	II (b, c, d, a, x[ 1], rolarray[3][3], 0x85845dd1);
  	II (a, b, c, d, x[ 8], rolarray[3][0], 0x6fa87e4f);
  	II (d, a, b, c, x[15], rolarray[3][1], 0xfe2ce6e0);
  	II (c, d, a, b, x[ 6], rolarray[3][2], 0xa3014314);
  	II (b, c, d, a, x[13], rolarray[3][3], 0x4e0811a1);
  	II (a, b, c, d, x[ 4], rolarray[3][0], 0xf7537e82);
  	II (d, a, b, c, x[11], rolarray[3][1], 0xbd3af235);
  	II (c, d, a, b, x[ 2], rolarray[3][2], 0x2ad7d2bb);
  	II (b, c, d, a, x[ 9], rolarray[3][3], 0xeb86d391);
	
	
	/**
	 * 结果重新备份，修改ABCD值 
	 */
  	state[0] += a;
  	state[1] += b;
  	state[2] += c;
  	state[3] += d;
}

/**
* 将bits数据编码成byte 
*
*/
void MD5::encode(const bit32* input, byte* output, size_t length) {
	int j = 0;
  	for (int i = 0; j < length; i++) {
    	output[j]= (byte)(input[i] | 0x00);
    	output[j + 1] = (byte)((input[i] >> 8) | 0x00);
   		output[j + 2] = (byte)((input[i] >> 16) | 0x00);
    	output[j + 3] = (byte)((input[i] >> 24) | 0x00);
    	j += 4; 
  	}
}

/**
 *  把byte换回 bits 数据 
 */
void MD5::decode(const byte* input, bit32* output, size_t length) {
	int j = 0;
  	for (int i = 0; j < length; i++) {
  		
    	output[i] = ((bit32)input[j]) | (((bit32)input[j + 1]) << 8) |
    	(((bit32)input[j + 2]) << 16) | (((bit32)input[j + 3]) << 24);
    	
    	 j += 4;
  	}
}


/**
 * 把散列值换成string 
 */
string MD5::toString() {
  	const byte* digest_ = getDigest();
  	string str;
  	str.reserve(16 << 1);
  	for (size_t i = 0; i < 16; ++i) {
    	int t = digest_[i];
    	int a = t / 16;
    	int b = t % 16;
    	str.append(1, HEX_NUMBERS[a]);
    	str.append(1, HEX_NUMBERS[b]);
  	}
 	return str;
}


/**
 * 输出函数 
 */
void printMD5(const string& message) {
	cout<<"信息("<<message<<")" <<"加密输出："
	<< MD5(message).toString()<<endl;
}
