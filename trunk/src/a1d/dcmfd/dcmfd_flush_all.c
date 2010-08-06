/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  Copyright (C) 2010 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "dcmfdimpl.h"

int A1DI_Flush_all()
{
    int result = DCMF_SUCCESS;
    int b,i,dst;
    DCMF_Request_t *request;
    DCQuad msginfo;  
    DCMF_Callback_t ack_callback; 
    int ack_count, pending_count; 
    size_t src_disp, dst_disp;

    A1U_FUNC_ENTER();

    request = (DCMF_Request_t *) malloc(sizeof(DCMF_Request_t) * a1_flushall_pending_limit);

    ack_count = 0;
    pending_count = 0;
    ack_callback.function = A1DI_Generic_done;
    ack_callback.clientdata = (void *) ack_count;

    if(a1_enable_scalefree_flush) {
       return result;
    }

    for(dst = 0; dst < A1D_Process_info.num_ranks; dst++) {
        if(dst != A1D_Process_info.my_rank) { 
          
          if(A1D_Connection_send_active[dst] > 0) {

             A1D_Control_flushack_info.rcv_active++;
             result = DCMF_Send(&A1D_Send_flush_info.protocol,
                           &request[pending_count],
                           A1D_Nocallback,
                           DCMF_SEQUENTIAL_CONSISTENCY,
                           dst,
                           0,
                           NULL,
                           &msginfo,
                           1);
             A1U_ERR_POP(result,"Send returned with an error \n");
             pending_count++;

          } else if (A1D_Connection_put_active[dst] > 0) {

             src_disp = (size_t)A1D_Put_Flushcounter_ptr[A1D_Process_info.my_rank] - 
                                   (size_t)A1D_Membase_global[A1D_Process_info.my_rank];
             dst_disp = (size_t)A1D_Put_Flushcounter_ptr[dst] - (size_t)A1D_Membase_global[dst] + 1;

             ack_count++;
             result = DCMF_Put(&A1D_Generic_put_protocol,
                      &request[pending_count],
                      A1D_Nocallback,
                      DCMF_SEQUENTIAL_CONSISTENCY,
                      dst,
                      1,
                      &A1D_Memregion_global[A1D_Process_info.my_rank],
                      &A1D_Memregion_global[dst],
                      src_disp,
                      dst_disp,
                      ack_callback);
             A1U_ERR_POP(result,"Send returned with an error \n");
             pending_count++;

          }

          if(pending_count >= a1_flushall_pending_limit) {
               while(A1D_Control_flushack_info.rcv_active > 0 || ack_count > 0) A1DI_Advance();
               pending_count = 0;
          }

        }
    }
    while(A1D_Control_flushack_info.rcv_active > 0 || ack_count > 0) A1DI_Advance();

    memset(A1D_Connection_send_active, 0, sizeof(uint32_t)*A1D_Process_info.num_ranks);
    memset(A1D_Connection_put_active, 0, sizeof(uint32_t)*A1D_Process_info.num_ranks);

    /*Reset the request pool*/
    A1DI_Reset_request_pool();

  fn_exit:
    A1DI_CRITICAL_EXIT();
    A1U_FUNC_EXIT();
    return result;

  fn_fail:
    goto fn_exit;
}

int A1D_Flush_all()
{
    int result = DCMF_SUCCESS;

    A1U_FUNC_ENTER();

    A1DI_CRITICAL_ENTER();

    result = A1DI_Flush_all();
    A1U_ERR_POP(result," a1di_flushall returned with an error \n");

  fn_exit:
    A1DI_CRITICAL_EXIT();
    A1U_FUNC_EXIT();
    return result;

  fn_fail:
    goto fn_exit;
}
