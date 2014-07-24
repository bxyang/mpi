#include <iostream>
#include <mpi.h>


int main(int argc, char* argv[]) {
    int rank, value, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    do {
        if (rank == 0) {
            fprintf(stderr, "Please give new value: ");
            scanf("%d", &value);
            if (size > 1) {
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                fprintf(stderr, "%d send %d to %d\n", rank, value, rank + 1);
            } else {
                fprintf(stderr, "only one process\n");
            }
        } else {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            fprintf(stderr, "%d recv %d to %d\n", rank, value, rank - 1);

            if (rank < size - 1) {
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                fprintf(stderr, "%d send %d to %d\n", rank, value, rank + 1);
            } else {
                fprintf(stderr, "last one recv\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    } while (value >= 0);

    MPI_Finalize();
    return 0;
}
