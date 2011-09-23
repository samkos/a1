/********************************************************************
 * The following is a notice of limited availability of the code, and disclaimer
 * which must be included in the prologue of the code and in all source listings
 * of the code.
 *
 * Copyright (c) 2010 Argonne Leadership Computing Facility, Argonne National Laboratory
 *
 * Permission is hereby granted to use, reproduce, prepare derivative works, and
 * to redistribute to others.
 *
 *                 LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer listed
 *    in this license in the documentation and/or other materials
 *    provided with the distribution.
 *
 *  - Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * The copyright holders provide no reassurances that the source code
 * provided does not infringe any patent, copyright, or any other
 * intellectual property rights of third parties.  The copyright holders
 * disclaim any liability to any recipient for claims brought against
 * recipient by any third party for infringement of that parties
 * intellectual property rights.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************/

#ifndef A1D_GLOBALS_H
#define A1D_GLOBALS_H

#include "a1d_headers.h"

typedef struct
{
    MPI_Comm comm;
    void ** addr_list;
#  ifndef NO_WINDOW_BOUNDS_CHECKING
    int * size_list; /* for correctness checking, technically optional */
#  endif
}
A1D_Window_t;

extern MPI_Comm A1D_COMM_WORLD;

extern int mpi_rank;
extern int mpi_size;

extern int * A1D_Put_flush_list;
extern int * A1D_Acc_flush_list;

extern void ** A1D_Baseptr_list;

#  ifdef __bgp__

extern DCMF_Memregion_t * A1D_Memregion_list;

extern DCMF_Protocol_t A1D_PutC_protocol;
extern DCMF_Protocol_t A1D_GetC_protocol;
extern DCMF_Protocol_t A1D_AccC_protocol;

extern DCMF_Protocol_t A1D_Inc32_protocol;
extern DCMF_Protocol_t A1D_Inc64_protocol;

extern DCMF_Callback_t A1D_Nocallback;

#define A1DI_Advance()                                             \
 do {                                                              \
         DCMF_Messager_advance(0);                                 \
    } while(0)                                                     \

#define A1DI_Conditional_advance(boolean)                           \
    do {                                                            \
          DCMF_Messager_advance(0);                                 \
    } while(boolean)                                                \

#  endif

#endif
