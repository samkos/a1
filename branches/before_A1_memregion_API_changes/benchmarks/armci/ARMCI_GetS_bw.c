/* 
 * The following is a notice of limited availability of the code, and disclaimer
 * which must be included in the prologue of the code and in all source listings
 * of the code.
 *
 * Copyright (c) 2010  Argonne Leadership Computing Facility, Argonne National
 * Laboratory
 *
 * Permission is hereby granted to use, reproduce, prepare derivative works, and
 * to redistribute to others.
 *
 *
 *                          LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer listed
 *   in this license in the documentation and/or other materials
 *   provided with the distribution.
 *
 * - Neither the name of the copyright holders nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <armci.h>
#include <mpi.h>

#define MAX_XDIM 1024 
#define MAX_YDIM 1024
#define ITERATIONS 1000
#define SKIP 10

int main(int argc, char *argv[])
{

    int i, j, rank, nranks, msgsize, dest;
    int xdim, ydim;
    long bufsize;
    double **buffer;
    double t_start, t_stop, t_total, d_total, bw;
    int count[2], src_stride, trg_stride, stride_level;
    int provided;
    armci_hdl_t handle;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

    ARMCI_Init_args(&argc, &argv);

    bufsize = MAX_XDIM * MAX_YDIM * sizeof(double);
    buffer = (double **) malloc(sizeof(double *) * nranks);
    ARMCI_Malloc((void **) buffer, bufsize);

    for (i = 0; i < bufsize / sizeof(double); i++)
    {
        *(buffer[rank] + i) = 1.0 + rank;
    }

    ARMCI_INIT_HANDLE(&handle);
    ARMCI_SET_AGGREGATE_HANDLE(&handle);

    ARMCI_Barrier();

    if (rank == 0)
    {
        printf("ARMCI_GetS Bandwidth in MBPS \n");
        printf("%30s %22s \n", "Dimensions(array of doubles)", "Latency");
        fflush(stdout);

        dest = 1;

        src_stride = MAX_YDIM * sizeof(double);
        trg_stride = MAX_YDIM * sizeof(double);
        stride_level = 1;

        for (xdim = 1; xdim <= MAX_XDIM; xdim *= 2)
        {

            count[1] = xdim;

            for (ydim = 1; ydim <= MAX_YDIM; ydim *= 2)
            {

                count[0] = ydim * sizeof(double);

                for (i = 0; i < ITERATIONS + SKIP; i++)
                {

                    if (i == SKIP) 
                          t_start = MPI_Wtime();

                    ARMCI_NbGetS((void *) buffer[dest],
                                 &src_stride,
                                 (void *) buffer[rank],
                                 &trg_stride,
                                 count,
                                 stride_level,
                                 1,
                                 &handle);

                }
                ARMCI_Wait(&handle);
                t_stop = MPI_Wtime();

                char temp[10];
                sprintf(temp, "%dX%d", xdim, ydim);
                t_total = t_stop - t_start;
                d_total = (xdim*ydim*sizeof(double)*ITERATIONS)/(1024*1024);
                bw = d_total/t_total;
                printf("%30s %20.2f \n", temp, bw);
                fflush(stdout);

                for (i = 0; i < xdim; i++)
                {
                    for (j = 0; j < ydim; j++)
                    {
                        if (*(buffer[rank] + i * MAX_XDIM + j) != (1.0 + dest))
                        {
                            printf("Data validation failed at X: %d Y: %d Expected : %f Actual : %f \n",
                                   i,
                                   j,
                                   (1.0 + dest),
                                   *(buffer[rank] + i * MAX_XDIM + j));
                            fflush(stdout);
                            return -1;
                        }
                    }
                }

                for (i = 0; i < bufsize / sizeof(double); i++)
                {
                    *(buffer[rank] + i) = 1.0 + rank;
                }
            }

        }

    }

    ARMCI_Barrier();

    ARMCI_UNSET_AGGREGATE_HANDLE(&handle);

    ARMCI_Free((void *) buffer[rank]);

    ARMCI_Finalize();

    MPI_Finalize();

    return 0;
}
