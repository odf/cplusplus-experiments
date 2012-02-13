#include <iostream>
#include "mpi.h"

int main(int argc, char **argv)
{
    MPI::Init(argc, argv);
    int rank = MPI::COMM_WORLD.Get_rank();
    int size = MPI::COMM_WORLD.Get_size();
    std::cout << "Hello world! I am " << rank << " of "
              << size << std::endl;
    MPI::Finalize();
    return 0;
}
