#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
    int rank, size, myn, i, N;
    double *vector, *myvec, mysum, total;

    MPI::Init(argc, argv);
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    // In the root process read the vector length, initialize
    // the vector and determine the sub-vector sizes
    if (rank == 0) {
        cout << "Enter the vector length: " << endl;
        cin >> N;
        vector = new double[N];
        for(i=0; i<N; i++)
            vector[i] = i;
        myn = N / size;
    }

    // Broadcast the local vector size
    MPI::COMM_WORLD.Bcast(&myn, 1, MPI::INT, 0);

    // allocate the local vectors in each process
    myvec = new double[myn];

    // Scatter the vector to all the processes
    MPI::COMM_WORLD.Scatter(vector, myn, MPI::DOUBLE, myvec, myn, MPI::DOUBLE,
                            0);
    for(i = 0; i < myn; i++)
        cout << "[" << rank << "]" << myvec[i] << endl;

    // Find the sum of all the elements of the local vector
    for (i = 0, mysum = 0; i < myn; i++)
        mysum += myvec[i];

    // Find the global sum of the vectors
    MPI::COMM_WORLD.Allreduce(&mysum, &total, 1, MPI::DOUBLE, MPI::SUM);

    // Multiply the local part of the vector by the global sum
    for (i = 0; i < myn; i++)
        myvec[i] *= total;

    // Gather the local vector in the root process
    MPI::COMM_WORLD.Gather(myvec, myn, MPI::DOUBLE, vector, myn, MPI::DOUBLE, 0);

    if (rank == 0)
        for(i=0; i<N; i++)
            cout << "[" << rank << "]" << vector[i] << endl;

    MPI::Finalize();
    return 0;
}
