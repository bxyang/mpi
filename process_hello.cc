/* vim: set ts=4 sw=4 sts=4 tw=100 */

#include <mpi.h>
#include <stdio.h>

void Hello();
int main(int argc, char* argv[]) {

    int my_rank, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if (comm_size < 2) {
        fprintf(stderr, "at least 2 processes should be provided\n");
    }

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    fprintf(stderr, "Process %d is alive on %s\n", my_rank, processor_name);
    MPI_Barrier(MPI_COMM_WORLD);
    Hello();
    MPI_Finalize();
}

void Hello() {
    int my_rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if (my_rank == 0) {
        fprintf(stdout, "Hello test from all to all\n");
        fflush(stdout);
    }
    
    int buf[2];
    MPI_Status status;
    for (int i = 0; i < comm_size; i++) {
        if (i == my_rank)
            continue;
        buf[0] = my_rank;
        buf[1] = i;
        MPI_Send(buf, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Recv(buf, 2, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        if (( buf[0] != i) || (buf[1] != my_rank)) {
            printf("Mismatch on hello process ids, me=%d, other=%d\n", my_rank, i);
        }
        printf("hello from %d to %d\n", my_rank, i);
        fflush(stdout);
    }
}
