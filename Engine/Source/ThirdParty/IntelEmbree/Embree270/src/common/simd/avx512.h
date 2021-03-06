// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../sys/platform.h"
#include "../sys/intrinsics.h"
#include "../math/constants.h"
//#include "sse_mic.h"

#include <zmmintrin.h>

#define _MM_SHUF_PERM(e3, e2, e1, e0) \
  ((_MM_PERM_ENUM)((e3)*64 + (e2)*16 + (e1)*4 + (e0)))

#define _MM_SHUF_PERM_NONE _MM_SHUF_PERM(3,2,1,0)

namespace embree 
{
  class bool16; 
  class int16; 
  class float16; 
}

#include "bool16_avx512.h"
#include "int16_avx512.h"
#include "float16_avx512.h"

namespace embree
{
  ////////////////////////////////////////////////////////////////////////////////
  /// Prefetching
  ////////////////////////////////////////////////////////////////////////////////

#define PFHINT_L1   0
#define PFHINT_L2   1
#define PFHINT_NT   2
#define PFHINT_L1EX 3
#define PFHINT_L2EX 4
#define PFHINT_NTEX 5

  template<const unsigned int mode>
    __forceinline void prefetch(const void * __restrict__ const m)
  {
    if (mode == PFHINT_L1)
      _mm_prefetch((const char*)m,_MM_HINT_T0); 
    else if (mode == PFHINT_L2) 
      _mm_prefetch((const char*)m,_MM_HINT_T1); 
    else if (mode == PFHINT_NT) 
      _mm_prefetch((const char*)m,_MM_HINT_NTA); 
    else if (mode == PFHINT_L1EX)
      _mm_prefetch((const char*)m,_MM_HINT_ET0);  
    else if (mode == PFHINT_L2EX) 
      _mm_prefetch((const char*)m,_MM_HINT_ET2); 
    else if (mode == PFHINT_NTEX) 
      _mm_prefetch((const char*)m,_MM_HINT_ENTA); 
  }
  
  __forceinline void gather_prefetch(const bool16 &m_active,
                                     const void *const ptr, 			     
                                     const int16 index, 
                                     const int mode = _MM_HINT_T2,
                                     const _MM_INDEX_SCALE_ENUM scale = _MM_SCALE_4,
                                     const _MM_UPCONV_PS_ENUM up = _MM_UPCONV_PS_NONE) 
  {
    _mm512_mask_prefetch_i32extgather_ps(index,m_active,ptr,up,scale,mode);
  }
  
  __forceinline void scatter_prefetch(const bool16 &m_active,
                                      void *const ptr, 			     
                                      const int16 index, 
                                      const int mode = _MM_HINT_ET2,
                                      const _MM_INDEX_SCALE_ENUM scale = _MM_SCALE_4,
                                      const _MM_UPCONV_PS_ENUM up = _MM_UPCONV_PS_NONE) 
  {
    _mm512_mask_prefetch_i32extscatter_ps(ptr,m_active,index,up,scale,mode);
  }
 

#if !defined(_MM_SHUF_PERM)
#define _MM_SHUF_PERM(e3, e2, e1, e0) ((_MM_PERM_ENUM)((e3)*64 + (e2)*16 + (e1)*4 + (e0)))
#define _MM_SHUF_PERM_NONE _MM_SHUF_PERM(3,2,1,0)
#endif

  template<const int D, const int C, const int B, const int A> 
    __forceinline float16 lshuf(const float16 &in)
  { 
    return _mm512_permute4f128_ps(in,(_MM_PERM_ENUM)_MM_SHUF_PERM(D,C,B,A));
  }


  template<const int D, const int C, const int B, const int A> 
    __forceinline float16 lshuf(const bool16 &mask, float16 &dest, const float16 &in)
  { 
    return _mm512_mask_permute4f128_ps(dest,mask,in,(_MM_PERM_ENUM) _MM_SHUF_PERM(D,C,B,A));
  }

  template<const int lane> 
    __forceinline float16 lane_shuffle_gather(const float16 &v0,const float16 &v1,const float16 &v2,const float16 &v3)
    {
      float16 t = lshuf<lane,lane,lane,lane>(v0);
      t = lshuf<lane,lane,lane,lane>(0xf0,t,v1);
      t = lshuf<lane,lane,lane,lane>(0xf00,t,v2);
      t = lshuf<lane,lane,lane,lane>(0xf000,t,v3);
      return t;
    }

  /* __forceinline float16 convert(const float4 &v) */
  /* { */
  /*   return broadcast4to16f(&v); */
  /* } */

  __forceinline int16 mul_uint64_t( const int16& a, const int16& b) { 
    const int16 low  = _mm512_mullo_epi32(a, b);
    const int16 high = _mm512_mulhi_epu32(a, b);
    return select(0x5555,low,high);
  }


}
