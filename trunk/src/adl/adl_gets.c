/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  Copyright (C) 2010 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "a1.h"
#include "a1d.h"
#include "a1u.h"

/* This is here because the build system does not yet have the necessary
 * logic to set these options for each device. */
#define A1D_IMPLEMENTS_GETS

#if defined A1D_IMPLEMENTS_GETS

int A1_GetS(int target,
            void* source_ptr,
            int *src_stride_ar,
            void* target_ptr,
            int *trg_stride_ar,
            int *count,
            int stride_levels)
{
    int status = A1_SUCCESS;

    A1U_FUNC_ENTER();

    /* FIXME: The profiling interface needs to go here */

    /* FIXME: Locking functionality needs to go here */

#   ifdef HAVE_ERROR_CHECKING
#   endif

    status = A1D_GetS(target,
                      source_ptr,
                      src_stride_ar,
                      target_ptr,
                      trg_stride_ar,
                      count,
                      stride_levels);
    A1U_ERR_POP(status!=A1_SUCCESS, "A1D_GetS returned error\n");

    fn_exit: A1U_FUNC_EXIT();
    return status;

    fn_fail: goto fn_exit;
}

#else

/* TODO: implement A1_GetS in terms of A1_Get for lazy port option */

#endif /* A1D_IMPLEMENTS_GETS */