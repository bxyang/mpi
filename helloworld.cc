// Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
// @file helloworld.cc
// @author yangbaoxing(yangbaoxing@baidu.com)
// @date 2014/04/24 18:38:24
// @version $Revision$ 
// @brief 
//

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
    fprintf(stderr," Hello World! Process %d of %d  on %s\n",
            myid,  numprocs, processor_name);
    MPI_Finalize();
}
