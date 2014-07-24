// Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
// @file square_vec.cc
// @author yangbaoxing(yangbaoxing@baidu.com)
// @date 2014/04/30 17:29:12
// @version $Revision$ 
// @brief 

/* vim: set ts=4 sw=4 sts=4 tw=100 */

#include <mpi.h>
#include <stdlib.h>

void compute_square(long int start, long int end, long long int* buf);
int get_start_end(int awail_process_num, long int n, long int* buf);
void get_recv_counts_and_offset(int* recv_counts, int* recv_counts,
        long int* start_end_buf, int num_process);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "invalid args!\n");
        return -1;
    }

    char* pend;
    long int n = strtol(argv[1], &pend, 10);
    if (n == 0) {
        fprintf(stderr, "invalid n %s\n", argv[1]);
        return -1;
    }
    int cur_id;
    int num_process;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &cur_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    int awail_process_num = num_process - 1;
    long int* root_start_end_buf;
    int used_process_num;
    
    if (cur_id == 0) {
        awail_process_num = num_process - 1;
        root_start_end_buf = new long int[2*num_process];
        root_start_end_buf[0] = 0;
        root_start_end_buf[1] = 0;
        used_process_num = get_start_end(awail_process_num, n, root_start_end_buf + 2);
    }
    long int leaf_start_end_buf[2];
    MPI_Scatter(root_start_end_buf, 2, MPI_LONG, leaf_start_end_buf, 2, MPI_LONG, 0, MPI_COMM_WORLD);

    long long int* square_send_buf;
    long long int* square_recv_buf;

    int* recv_counts;
    int* recv_offset;
    if (cur_id != 0) {
        square_send_buf = new long long int[leaf_start_end_buf[1] - leaf_start_end_buf[0]];
        compute_square(leaf_start_end_buf[0], leaf_start_end_buf[1], square_send_buf);
    } else {
        recv_counts = new int[num_process];
        recv_offset = new int[num_process];
        square_recv_buf = new long long int[n];
        get_recv_counts_and_offset(recv_counts, recv_offset, root_start_end_buf, num_process);
    }


    MPI_Gatherv(square_send_buf, leaf_start_end_buf[1] - leaf_start_end_buf[0], MPI_LONG_LONG, square_recv_buf,
            recv_counts, recv_offset, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (cur_id == 0) {
        fprintf(stdout, "square: ");
        for (int i = 0; i < n; i++) {
            fprintf(stdout, "%lld, ", square_recv_buf[i]);
        }
        fprintf(stdout, "\n");
    }
    MPI_Finalize();
}

void write_to_file() {

}

void get_recv_counts_and_offset(int* recv_counts, int* recv_offset,
    long int* start_end_buf, int num_process) {
    recv_counts[0] = start_end_buf[1] - start_end_buf[0];
    recv_offset[0] = 0;
    for (int i = 1; i < num_process; i++) {
        recv_counts[i] = start_end_buf[2*i+1] - start_end_buf[2*i];
        recv_offset[i] = recv_offset[i-1] + recv_counts[i-1];
    }
}
void compute_square(long int start, long int end, long long int* buf) {
    for (int i = start; i < end; i++)
        buf[i-start] = i * i;
}

int get_start_end(int awail_process_num, long int n, long int* buf) {
    int incre = n / awail_process_num;
    int rem = n % awail_process_num;
    
    long int start = 1;
    long int end = start + incre + (rem == 0 ? 0 : 1);

    int used_process_num = awail_process_num;
    if (incre == 0) used_process_num = rem;

    for (int i = 1; i <= used_process_num; i++) {
        *(buf++) = start;
        *(buf++) = end;
        start = end;
        if (i <= rem) {
            end = start + incre + 1;
        } else {
            end = start + incre;
        }
        if (end > (n + 1)) end = n + 1;
    }

    for (int i = used_process_num + 1; i <= awail_process_num; i++) {
        *(buf++) = 0;
        *(buf++) = 0;
    }
    return used_process_num;
}
