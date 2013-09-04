#ifndef _HTYPE_
#define _HTYPE_

#define    NULL 0

#define		u8		unsigned char
#define		u16		unsigned short
#define		u32		unsigned long

#define		s8		signed char
#define		s16		signed short
#define		s32		signed long

typedef unsigned int   uint; //32bits
typedef unsigned short ushort; //16bits
typedef unsigned long  ulong; //32bits
typedef unsigned char  uchar; //8bits


#define get_unaligned(ptr) (*(ptr))

#define put_unaligned(val, ptr) ((void)( *(ptr) = (val) ))

#define bes2cpu(x) \
	((u16)((((u16)(x) & 0x00FFU) << 8) | \
		  (((u16)(x) & 0xFF00U) >> 8)))
 
#define bel2cpu(x) \
	((u32)((((u32)(x) & 0x000000FFU) << 24) | \
		  (((u32)(x) & 0x0000FF00U) <<  8) | \
		  (((u32)(x) & 0x00FF0000U) >>  8) | \
		  (((u32)(x) & 0xFF000000U) >> 24)))

#define bell2cpu(x) \
	((u64)((((u64)(x) & 0x00000000000000FFULL) << 56) | \
		  (((u64)(x) & 0x000000000000FF00ULL) << 40) | \
		  (((u64)(x) & 0x0000000000FF0000ULL) << 24) | \
		  (((u64)(x) & 0x00000000FF000000ULL) <<  8) | \
		  (((u64)(x) & 0x000000FF00000000ULL) >>  8) | \
		  (((u64)(x) & 0x0000FF0000000000ULL) >> 24) | \
		  (((u64)(x) & 0x00FF000000000000ULL) >> 40) | \
		  (((u64)(x) & 0xFF00000000000000ULL) >> 56)))		

#define les2cpu(x) (x)
#define lel2cpu(x) (x)
#define lell2cpu(x) (x)

#define cpus2le(x) (x)
#define cpul2le(x) (x)
#define cpull2le(x) (x)

#define cpus2be(x)  (bes2cpu(x))
#define cpul2be(x)   (bel2cpu(x))
#define cpull2be(x)  (bell2cpu(x))

#endif
