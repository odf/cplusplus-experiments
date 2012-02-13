#include <boost/mpi.hpp>
#include <iostream>

namespace mpi = boost::mpi;

template<typename T>
T add(const T a, const T b)
{
    return a + b;
}

int main(int argc, char* argv[])
{
    int rank, myn, i, N;
    double *vector, *myvec, mysum, total;

    mpi::environment env(argc, argv);
    mpi::communicator world;

    rank = world.rank();

    // In the root process read the vector length, initialize
    // the vector and determine the sub-vector sizes
    if (rank == 0)
    {
        std::cout << "Enter the vector length: " << std::endl;
        std::cin >> N;
        vector = new double[N];
        for (i = 0; i < N; ++i)
        {
            vector[i] = i;
        }
        myn = N / world.size();
    }

    // Broadcast the local vector size
    mpi::broadcast(world, myn, 0);

    // allocate the local vectors in each process
    myvec = new double[myn];

    // Scatter the vector to all the processes
    mpi::scatter(world, vector, myvec, myn, 0);

    for(i = 0; i < myn; i++)
    {
        std::cout << "[" << rank << "]" << myvec[i] << std::endl;
    }

    // Find the sum of all the elements of the local vector
    for (i = 0, mysum = 0; i < myn; i++)
    {
        mysum += myvec[i];
    }

    // Find the global sum of the vectors
    mpi::all_reduce(world, mysum, total, add<double>);

    // Multiply the local part of the vector by the global sum
    for (i = 0; i < myn; i++)
    {
        myvec[i] *= total;
    }

    // Gather the local vector in the root process
    mpi::gather(world, myvec, myn, vector, 0);

    if (rank == 0)
    {
        for(i=0; i<N; i++)
        {
            std::cout << "[" << rank << "]" << vector[i] << std::endl;
        }
    }

    return 0;
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k boost_mpi_test"
** End:
*/
