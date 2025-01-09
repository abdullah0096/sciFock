#include "include/utils.hpp"
#include "include/constants.hpp"

#include <NTL/mat_ZZ.h>

#include <iostream>

using namespace std;
using namespace NTL;

void N_mul()
{
    try
    {
        openvdb::DoubleGrid::Ptr overlap, density;

        openvdb::initialize();
        overlap = openvdb::DoubleGrid::create(0.0);
        density = openvdb::DoubleGrid::create(0.0);

        openvdb::DoubleGrid::Accessor d_accessor = density->getAccessor();
        openvdb::DoubleGrid::Accessor o_accessor = overlap->getAccessor();

        d_accessor.setValue(openvdb::Coord(1312, 1312, 0), 0);
        o_accessor.setValue(openvdb::Coord(1312, 1312, 0), 0);

        cout << "\n Reading density :: ";
        utils::readLowerTriangularMatrix(N_1312_P, density);

        cout << "\n Reading overlap :: ";
        utils::readLowerTriangularMatrix(N_1312_S, overlap);

        std::cout << "Matrices Loaded Successfully." << std::endl;

        // Matrix multiplication
        auto start_ans = std::chrono::high_resolution_clock::now();
        openvdb::DoubleGrid::Ptr ans = utils::multiplySymmetricGrids_activeVoxel(density, overlap);
        auto end_ans = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_ans - start_ans;
        cout << "\n P*S multiplication done \t time :: " << duration.count() << " seconds\n";

        // --------------------
        // mat_RR overlap, density;
        // overlap = utils::readLowerTriangularMatrix_NTL(N_4445_S);
        // density = utils::readLowerTriangularMatrix_NTL(N_4445_P);

        // std::cout << "Matrices Loaded Successfully." << std::endl;

        // // Matrix multiplication
        // auto start_ans = std::chrono::high_resolution_clock::now();
        // mat_RR ans = density * overlap;
        // auto end_ans = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end_ans - start_ans;
        // cout << "\n P*S multiplication done \t time :: " << duration.count() << " seconds\n";

        // RR S_trace = utils::computeTrace_NTL(ans);
        // cout << "\n S_trace :: " << S_trace << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
}

void S_mul()
{
    try
    {
        openvdb::DoubleGrid::Ptr fock, overlap, density;

        overlap = utils::readMatrixMarketFileWithCutoff_i_j_val(S_8400_S, cutOffMinus10);
        density = utils::readMatrixMarketFileWithCutoff_i_j_val(S_8400_P, cutOffMinus10);

        if (density && overlap)
        {
            std::cout << "Both matrices successfully loaded. Starting multiplication..." << std::endl;
            {
                auto start_ans = std::chrono::high_resolution_clock::now();
                openvdb::DoubleGrid::Ptr resultMatrix = utils::multiplySymmetricGrids(density, overlap);
                auto end_ans = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end_ans - start_ans;
                cout << "\n P*S multiplication done \t time :: " << duration.count() << " seconds\n";
                // Trace of P*S matrix
                double S_trace = utils::computeTrace(resultMatrix);
                cout << "\n S_trace :: " << S_trace << endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
}

void S_mul_NTL()
{
    try
    {
        mat_RR overlap, density;

        overlap = utils::readMatrixMarketFileWithCutoff_i_j_val_NTL(S_8400_S, cutOffMinus10);
        cout << "\n Overlap matrix read - complete...\n";
        density = utils::readMatrixMarketFileWithCutoff_i_j_val_NTL(S_8400_P, cutOffMinus10);
        cout << "\n Density matrix read - complete...\n";

        std::cout << "Both matrices successfully loaded. Starting multiplication..." << std::endl;
        {
            auto start_ans = std::chrono::high_resolution_clock::now();
            mat_RR resultMatrix = density * overlap;
            auto end_ans = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_ans - start_ans;
            cout << "\n P*S multiplication done \t time :: " << duration.count() << " seconds\n";
            // Trace of P*S matrix
            RR S_trace = utils::computeTrace_NTL(resultMatrix);
            cout << "\n S_trace :: " << S_trace << endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
}

int main()
{
    try
    {
        N_mul();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}