#include <iostream>
#include <mpi.h>

using namespace std;

double Trap(double left_endpt, double right_endpt, int count, double base_len) {
    double estimate = (left_endpt + right_endpt) / 2.0;
    double x;

    for (int i = 1; i < count - 1; i++) {
        x = left_endpt + i * base_len;
        estimate += x;
    }

    estimate *= base_len;

    return estimate;
}

int main(void) {
    double a = 0.0, b = 3.0, h;
    int localN, n = 1024, myRank, CommonSize;
    double local_int, local_total = 0.0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &CommonSize);

    h = (b - a) / n;
    localN = n / CommonSize;
    double localA = a + localN * myRank * h;
    double localB = localA + localN * h;
    local_int = Trap(localA, localB, localN, h);

    if (myRank != 0) {
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        local_total = local_int;

        for (int i = 1; i < CommonSize; i++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_total += local_int;
        }
    }

    if (myRank == 0) {
        cout << "the trapezoids with n is :" << n << endl;
        cout << "the trapezoids with a is :" << a << endl;
        cout << "the trapezoids with b is :" << b << endl;
        cout << "the trapezoids with h is :" << h << endl;
        cout << "the trapezoids with localA is : " << localA << endl;
        cout << "the trapezoids with localB is :" << localB << endl;
        cout << "the trapezoids with local_total is :" << local_total << endl;
    }

    MPI_Finalize();
    return 0;
}

