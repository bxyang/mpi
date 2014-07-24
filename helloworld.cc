
// mpic++ -I ~/var/openmpi/include/ helloworld.cc

#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]){

    int myid, numprocs;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Get_processor_name(processor_name,&namelen);

    int version, subversion;
    MPI_Get_version(&version, &subversion);
    fprintf(stderr," Hello World! Process %d of %d  on %s\n",
            myid,  numprocs, processor_name);
    fprintf(stderr," Hello World! MPI version %d, subversion %d\n",
            version, subversion);
    MPI_Finalize();
}
