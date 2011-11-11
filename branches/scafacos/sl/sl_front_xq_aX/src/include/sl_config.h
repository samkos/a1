#ifndef __SL_CONFIG_H__
#define __SL_CONFIG_H__


#include "fortran2c_types.h"
#include "config_types.h"


/* enable/disable MPI */
/*#define SL_USE_MPI*/


/* standard (SL) integer data type */
#define sl_int_type_c             SL_INTEGER_C
#define sl_int_type_mpi           SL_INTEGER_MPI
#define sl_int_size_mpi           1
#define sl_int_type_fmt           SL_INTEGER_FMT


/* key section */
#define sl_key_type_c             INTEGER_C
#define sl_key_type_mpi           INTEGER_MPI
#define sl_key_size_mpi           1

#define sl_key_integer
#define sl_key_type_fmt           INTEGER_FMT

extern INTEGER_C key_mask;

#define sl_key_purify(_k_)        (_k_ & key_mask)


/* data section */
#define SL_DATA0                  /* xyz */
#define sl_data0_type_c           REAL_C
#define sl_data0_size_c           3
#define sl_data0_type_mpi         REAL_MPI
#define sl_data0_size_mpi         3

#define SL_DATA1                  /* q */
#define sl_data1_type_c           REAL_C
#define sl_data1_size_c           1
#define sl_data1_type_mpi         REAL_MPI
#define sl_data1_size_mpi         1

/*#define SL_DATA2*/                  /* scr */
#define sl_data2_type_c           INTEGER_C
#define sl_data2_size_c           1
#define sl_data2_type_mpi         INTEGER_MPI
#define sl_data2_size_mpi         1

extern int front_aX;

#define SL_DATA3                  /* addr */
#define sl_data3_type_c           char
#define sl_data3_size_c           front_aX
#define sl_data3_type_mpi         MPI_CHAR
#define sl_data3_size_mpi         front_aX

#define sl_data3_flex
#define sl_data3_memcpy

#endif /* __SL_CONFIG_H__ */