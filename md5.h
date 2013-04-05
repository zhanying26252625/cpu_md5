#ifndef MD5_H
#define MD5_H

#include <string.h>
#include <string>
#include <fstream>

/* Type define */
//Since int is always 32bits on no matter x86 or x64
typedef unsigned char byte;
typedef unsigned long ulong;
typedef unsigned int uInt;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5 {
public:
	MD5();
	string calMD5FromString(const string& str);
    string calMD5FromFile(const string& fileName);
private:
	void init();
    void update(const byte *input, size_t length);
    const byte* digest();
	void final();
	void transform(const byte block[64]);
	void encode(const uInt *input, byte *output, size_t length);
	void decode(const byte *input, uInt *output, size_t length);
	string bytesToHexString(const byte *input, size_t length);
private:
	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);
private:
	uInt _state[4];	/* state (ABCD) */
	uInt _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024*16;
};

#endif/*MD5_H*/
