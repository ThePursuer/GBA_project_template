#ifndef FAST_H
#define FAST_H


#include <cstdint>

#include <gba.h>

extern "C" {


//!	Fast-fill by words, analogous to memset()
/*! Like CpuFastSet(), only without the requirement of 
	  32byte chunks and no awkward store-value-in-memory-first issue.
	\param dst	Destination address.
	\param wd	Fill word (not address).
	\param wdcount	Number of words to fill.
	\note	\a dst <b>must</b> be word aligned.
	\note \a r0 returns as \a dst + \a wdcount*4.
*/
void memset32(void *dst, uint32_t wd, uint32_t wdcount);
void memset16(void *dst, u16 src, u32 hwn);

//!	\brief Fast-copy by words.
/*! Like CpuFastFill(), only without the requirement of 32byte chunks
	\param dst	Destination address.
	\param src	Source address.
	\param wdcount	Number of words.
	\note	\a src and \a dst <b>must</b> be word aligned.
	\note	\a r0 and \a r1 return as 
	  \a dst + \a wdcount*4 and \a src + \a wdcount*4.
*/
void memcpy32(void *dst, const void* src, uint32_t wdcount);
void memcpy16(void *dst, const void *src, u32 hwn);

}

#endif // FAST_H