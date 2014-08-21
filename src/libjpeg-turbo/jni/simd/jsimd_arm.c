/*
 * jsimd_arm.c
 *
 * Copyright 2009 Pierre Ossman <ossman@cendio.se> for Cendio AB
 * Copyright 2009-2011 D. R. Commander
 * 
 * Based on the x86 SIMD extension for IJG JPEG library,
 * Copyright (C) 1999-2006, MIYASAKA Masaru.
 * For conditions of distribution and use, see copyright notice in jsimdext.inc
 *
 * This file contains the interface between the "normal" portions
 * of the library and the SIMD implementations when running on
 * ARM architecture.
 *
 * Based on the stubs from 'jsimd_none.c'
 */

#define JPEG_INTERNALS
#include "../jinclude.h"
#include "../jpeglib.h"
#include "../jsimd.h"
#include "../jdct.h"
#include "../jsimddct.h"
#include "jsimd.h"


#include <stdio.h>
// #include <link.h>
#include <elf.h>
//#include <asm/hwcap.h>
#include <string.h>
#include <ctype.h>

static unsigned int simd_support = ~0;

/* volatile ElfW(auxv_t) *auxv=NULL; */

LOCAL(void) libjpeg_arch_specific_init(void);
LOCAL(void) init_simd (void);

GLOBAL(void __attribute__ ((constructor))) libjpeg_general_init(void);

/*
LOCAL(ElfW(auxv_t) *)get_auxv(void)
{
  FILE *auxv_f;
  ElfW(auxv_t) auxv_struct;
  int i = 0;

  if(auxv == NULL) {
    auxv_f = fopen("/proc/self/auxv", "r");

    if(auxv_f == 0) {
       perror("Error opening file for reading");
          return 0;
    }   
    auxv =(ElfW(auxv_t) *)malloc(getpagesize());

    do  
      {   
      fread(&auxv_struct, sizeof(ElfW(auxv_t)), 1, auxv_f);
      auxv[i] = auxv_struct;
      i++;
      } while(auxv_struct.a_type != AT_NULL);
  }
  return auxv;
}
*/

GLOBAL(void __attribute__ ((constructor))) libjpeg_general_init(void)
{
  // Architecture independent library init

//  get_auxv();

  // call to arch specific init
  libjpeg_arch_specific_init();
}

LOCAL(void) libjpeg_arch_specific_init(void) 
{

  init_simd();

}

/*
 * Check what SIMD accelerations are supported.
 *
 */
LOCAL(void)
init_simd (void)
{
  char *env = NULL;

  if (simd_support != ~0)
    return;

  simd_support = 0;

#ifdef __ARM_HAVE_NEON
    simd_support |= JSIMD_ARM_NEON;
#endif


  /* Force different settings through environment variables */
  env = getenv("JSIMD_FORCE_ARM_NEON");
  if ((env != NULL) && (strcmp(env, "1") == 0))
    simd_support &= JSIMD_ARM_NEON;
  env = getenv("JSIMD_FORCE_NO_SIMD");
  if ((env != NULL) && (strcmp(env, "1") == 0))
    simd_support = 0;
}

GLOBAL(int)
jsimd_can_rgb_ycc (void)
{
  /* The code is optimised for these values only */
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if ((RGB_PIXELSIZE != 3) && (RGB_PIXELSIZE != 4))
    return 0;

  if (simd_support & JSIMD_ARM_NEON)
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_rgb_gray (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_ycc_rgb (void)
{
  /* The code is optimised for these values only */
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if ((RGB_PIXELSIZE != 3) && (RGB_PIXELSIZE != 4))
    return 0;
  if (simd_support & JSIMD_ARM_NEON)
    return 1;

  return 0;
}

GLOBAL(void)
jsimd_rgb_ycc_convert (j_compress_ptr cinfo,
                       JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
                       JDIMENSION output_row, int num_rows)
{
  void (*neonfct)(JDIMENSION, JSAMPARRAY, JSAMPIMAGE, JDIMENSION, int);

  switch(cinfo->in_color_space)
  {
    case JCS_EXT_RGB:
      neonfct=jsimd_extrgb_ycc_convert_neon;
      break;
    case JCS_EXT_RGBX:
    case JCS_EXT_RGBA:
      neonfct=jsimd_extrgbx_ycc_convert_neon;
      break;
    case JCS_EXT_BGR:
      neonfct=jsimd_extbgr_ycc_convert_neon;
      break;
    case JCS_EXT_BGRX:
    case JCS_EXT_BGRA:
      neonfct=jsimd_extbgrx_ycc_convert_neon;
      break;
    case JCS_EXT_XBGR:
    case JCS_EXT_ABGR:
      neonfct=jsimd_extxbgr_ycc_convert_neon;
      break;
    case JCS_EXT_XRGB:
    case JCS_EXT_ARGB:
      neonfct=jsimd_extxrgb_ycc_convert_neon;
      break;
    default:
      neonfct=jsimd_extrgb_ycc_convert_neon;
      break;
  }

  if (simd_support & JSIMD_ARM_NEON)
    neonfct(cinfo->image_width, input_buf,
        output_buf, output_row, num_rows);
}

GLOBAL(void)
jsimd_rgb_gray_convert (j_compress_ptr cinfo,
                        JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
                        JDIMENSION output_row, int num_rows)
{
}

GLOBAL(void)
jsimd_ycc_rgb_convert (j_decompress_ptr cinfo,
                       JSAMPIMAGE input_buf, JDIMENSION input_row,
                       JSAMPARRAY output_buf, int num_rows)
{
  void (*neonfct)(JDIMENSION, JSAMPIMAGE, JDIMENSION, JSAMPARRAY, int);

  switch(cinfo->out_color_space)
  {
    case JCS_EXT_RGB:
      neonfct=jsimd_ycc_extrgb_convert_neon;
      break;
    case JCS_EXT_RGBX:
    case JCS_EXT_RGBA:
      neonfct=jsimd_ycc_extrgbx_convert_neon;
      break;
    case JCS_EXT_BGR:
      neonfct=jsimd_ycc_extbgr_convert_neon;
      break;
    case JCS_EXT_BGRX:
    case JCS_EXT_BGRA:
      neonfct=jsimd_ycc_extbgrx_convert_neon;
      break;
    case JCS_EXT_XBGR:
    case JCS_EXT_ABGR:
      neonfct=jsimd_ycc_extxbgr_convert_neon;
      break;
    case JCS_EXT_XRGB:
    case JCS_EXT_ARGB:
      neonfct=jsimd_ycc_extxrgb_convert_neon;
      break;
  default:
      neonfct=jsimd_ycc_extrgb_convert_neon;
      break;
  }

  if (simd_support & JSIMD_ARM_NEON)
    neonfct(cinfo->output_width, input_buf,
        input_row, output_buf, num_rows);
}

GLOBAL(int)
jsimd_can_h2v2_downsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_downsample (void)
{
  return 0;
}

GLOBAL(void)
jsimd_h2v2_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
                       JSAMPARRAY input_data, JSAMPARRAY output_data)
{
}

GLOBAL(void)
jsimd_h2v1_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
                       JSAMPARRAY input_data, JSAMPARRAY output_data)
{
}

GLOBAL(int)
jsimd_can_h2v2_upsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_upsample (void)
{
  return 0;
}

GLOBAL(void)
jsimd_h2v2_upsample (j_decompress_ptr cinfo,
                     jpeg_component_info * compptr, 
                     JSAMPARRAY input_data,
                     JSAMPARRAY * output_data_ptr)
{
}

GLOBAL(void)
jsimd_h2v1_upsample (j_decompress_ptr cinfo,
                     jpeg_component_info * compptr, 
                     JSAMPARRAY input_data,
                     JSAMPARRAY * output_data_ptr)
{
}

GLOBAL(int)
jsimd_can_h2v2_fancy_upsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_fancy_upsample (void)
{
  return 0;
}

GLOBAL(void)
jsimd_h2v2_fancy_upsample (j_decompress_ptr cinfo,
                           jpeg_component_info * compptr, 
                           JSAMPARRAY input_data,
                           JSAMPARRAY * output_data_ptr)
{
}

GLOBAL(void)
jsimd_h2v1_fancy_upsample (j_decompress_ptr cinfo,
                           jpeg_component_info * compptr, 
                           JSAMPARRAY input_data,
                           JSAMPARRAY * output_data_ptr)
{
}

GLOBAL(int)
jsimd_can_h2v2_merged_upsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_merged_upsample (void)
{
  return 0;
}

GLOBAL(void)
jsimd_h2v2_merged_upsample (j_decompress_ptr cinfo,
                            JSAMPIMAGE input_buf,
                            JDIMENSION in_row_group_ctr,
                            JSAMPARRAY output_buf)
{
}

GLOBAL(void)
jsimd_h2v1_merged_upsample (j_decompress_ptr cinfo,
                            JSAMPIMAGE input_buf,
                            JDIMENSION in_row_group_ctr,
                            JSAMPARRAY output_buf)
{
}

GLOBAL(int)
jsimd_can_convsamp (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if (sizeof(DCTELEM) != 2)
    return 0;

  if (simd_support & JSIMD_ARM_NEON)
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_convsamp_float (void)
{
  return 0;
}

GLOBAL(void)
jsimd_convsamp (JSAMPARRAY sample_data, JDIMENSION start_col,
                DCTELEM * workspace)
{
  if (simd_support & JSIMD_ARM_NEON)
    jsimd_convsamp_neon(sample_data, start_col, workspace);
}

GLOBAL(void)
jsimd_convsamp_float (JSAMPARRAY sample_data, JDIMENSION start_col,
                      FAST_FLOAT * workspace)
{
}

GLOBAL(int)
jsimd_can_fdct_islow (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_fdct_ifast (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (sizeof(DCTELEM) != 2)
    return 0;

  if (simd_support & JSIMD_ARM_NEON)
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_fdct_float (void)
{
  return 0;
}

GLOBAL(void)
jsimd_fdct_islow (DCTELEM * data)
{
}

GLOBAL(void)
jsimd_fdct_ifast (DCTELEM * data)
{
  if (simd_support & JSIMD_ARM_NEON)
    jsimd_fdct_ifast_neon(data);
}

GLOBAL(void)
jsimd_fdct_float (FAST_FLOAT * data)
{
}

GLOBAL(int)
jsimd_can_quantize (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (sizeof(JCOEF) != 2)
    return 0;
  if (sizeof(DCTELEM) != 2)
    return 0;

  if (simd_support & JSIMD_ARM_NEON)
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_quantize_float (void)
{
  return 0;
}

GLOBAL(void)
jsimd_quantize (JCOEFPTR coef_block, DCTELEM * divisors,
                DCTELEM * workspace)
{
  if (simd_support & JSIMD_ARM_NEON)
    jsimd_quantize_neon(coef_block, divisors, workspace);
}

GLOBAL(void)
jsimd_quantize_float (JCOEFPTR coef_block, FAST_FLOAT * divisors,
                      FAST_FLOAT * workspace)
{
}

GLOBAL(int)
jsimd_can_idct_2x2 (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (sizeof(JCOEF) != 2)
    return 0;
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if (sizeof(ISLOW_MULT_TYPE) != 2)
    return 0;

  if ((simd_support & JSIMD_ARM_NEON))
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_idct_4x4 (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (sizeof(JCOEF) != 2)
    return 0;
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if (sizeof(ISLOW_MULT_TYPE) != 2)
    return 0;

  if ((simd_support & JSIMD_ARM_NEON))
    return 1;

  return 0;
}

GLOBAL(void)
jsimd_idct_2x2 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  if ((simd_support & JSIMD_ARM_NEON))
    jsimd_idct_2x2_neon(compptr->dct_table, coef_block, output_buf, output_col);
}

GLOBAL(void)
jsimd_idct_4x4 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  if ((simd_support & JSIMD_ARM_NEON))
    jsimd_idct_4x4_neon(compptr->dct_table, coef_block, output_buf, output_col);
}

GLOBAL(int)
jsimd_can_idct_islow (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (sizeof(JCOEF) != 2)
    return 0;
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if (sizeof(ISLOW_MULT_TYPE) != 2)
    return 0;

  if (simd_support & JSIMD_ARM_NEON)
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_idct_ifast (void)
{
  /* The code is optimised for these values only */
  if (DCTSIZE != 8)
    return 0;
  if (sizeof(JCOEF) != 2)
    return 0;
  if (BITS_IN_JSAMPLE != 8)
    return 0;
  if (sizeof(JDIMENSION) != 4)
    return 0;
  if (sizeof(IFAST_MULT_TYPE) != 2)
    return 0;
  if (IFAST_SCALE_BITS != 2)
    return 0;

  if ((simd_support & JSIMD_ARM_NEON))
    return 1;

  return 0;
}

GLOBAL(int)
jsimd_can_idct_float (void)
{
  return 0;
}

GLOBAL(void)
jsimd_idct_islow (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  if ((simd_support & JSIMD_ARM_NEON))
    jsimd_idct_islow_neon(compptr->dct_table, coef_block, output_buf, output_col);
}

GLOBAL(void)
jsimd_idct_ifast (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  if ((simd_support & JSIMD_ARM_NEON))
    jsimd_idct_ifast_neon(compptr->dct_table, coef_block, output_buf, output_col);
}

GLOBAL(void)
jsimd_idct_float (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
}

