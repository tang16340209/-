/**
 * 中山大学数据科学与计算机学院  16级软件工程电子政务方向 
 * author：唐育涛 16340209 
 * md5码的设计实现 
 */

#ifndef MD5_H
#define MD5_H

/* Parameters of MD5. */
const unsigned int rolarray[4][4] = {
        { 7, 12, 17, 22 },
        { 5, 9, 14, 20 },
		{ 4, 11, 16, 23 },
     	{ 6, 10, 15, 21 }
};
/**
 * MD5 压缩函数HMD5
 * 4轮循环中使用的生成
 * 函数(轮函数) g 的定义
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/**
 * <<<s : 将32位输入循环左移(CLS) s 位。
 */
#define CLS(num, n) (((num) << (n)) | ((num) >> (32-(n))))
/**
 * 每轮循环中的一步运算逻辑
 * a <-b + ((a + g(b,c,d) + X[k] + T[i]) <<<s)
 * 说明：
 * a, b, c, d : MD 缓冲区(A, B, C, D) 的当前值。
 * g : 轮函数(F, G, H, I 中的一个)。
 *  <<s : 将32位输入循环左移(CLS) s 位。
 * X[k] : 当前处理消息分组的第k 个32位字，即 M [qx16+k]
 * T[i] : T 表的第i 个元素，32位字。
 * + : 模232 加法。
 */
#define FF(a,b,c,d,x,s,ti) { a+=F(b,c,d)+x+ti; a=CLS(a,s); a+=b;}
#define GG(a,b,c,d,x,s,ti) { a+=G(b,c,d)+x+ti; a=CLS(a,s); a+=b;}
#define HH(a,b,c,d,x,s,ti) { a+=H(b,c,d)+x+ti; a=CLS(a,s); a+=b;}
#define II(a,b,c,d,x,s,ti) { a+=I(b,c,d)+x+ti; a=CLS(a,s); a+=b;}

#include <string>
#include <cstring>

using std::string;

typedef unsigned char byte;
typedef unsigned int bit32;

class MD5 {
public:

  MD5(const string& message);

  const byte* getDigest();

  string toString();

private:

  void init(const byte* input, size_t len);
  void transform(const byte block[64]);
  void encode(const bit32* input, byte* output, size_t length);
  void decode(const byte* input, bit32* output, size_t length);

private:
 
  bool finished;  //是否完成的标记 
  byte buffer[64]; //输入Buffer 
  byte digest[16]; // MD结果 
  bit32 state[4]; //寄存器 A,B,C,D 
  bit32 count[2]; //64位长度，记录信息长度 

  static const byte PADDING[64]; //用于填充 
  static const char HEX_NUMBERS[16]; //十六进制表示 
};

#endif // MD5_H
