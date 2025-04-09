#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdexcept>

// Kieran Egan
// EECS348
// Lab 9
// Matrix Operations

// Simple Matrix class
template <typename T>
class Matrix {
private:
    int N; // dimension: matrix is N x N
    T* data; // pointer to dynamically allocated data

public:

    // constructors/destructor
    Matrix(int size = 0) : N(size), data(nullptr) {
        if (N > 0) {
            data = new T[N * N];
            for(int i = 0; i < N*N; i++){
                data[i] = T();
            }
        }
    }

    // copy constructor
    Matrix(const Matrix<T>& other) : N(other.N), data(nullptr) {
        if(N > 0){
            data = new T[N * N];
            for (int i = 0; i < N*N; i++){
                data[i] = other.data[i];
            }
        }
    }

    // destructor
    ~Matrix() {
        delete[] data;
    }

    // copy assignment operator
    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this == &other) return *this; // self-assign check

        // clean data
        delete[] data;
        N = other.N;
        if(N > 0) {
            data = new T[N * N];
            for (int i = 0; i < N*N; i++){
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
        return *this;
    }

    // getters
    int size() const { return N; }

    // accesses elements (row, col)
    T& operator()(int row, int col) {
        return data[row * N + col];
    }
    const T& operator()(int row, int col) const {
        return data[row * N + col];
    }

    // operator overloading

    // 1) matrix addition
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (N != other.N) {
            throw std::runtime_error("Matrix dimensions do not match for addition!");
        }
        Matrix<T> result(N);
        for(int i = 0; i < N*N; i++){
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    // 2) matrix multiplication
    Matrix<T> operator*(const Matrix<T>& other) const {
        if (N != other.N) {
            throw std::runtime_error("Matrix dimensions do not match for multiplication!");
        }
        Matrix<T> result(N);
        for(int r = 0; r < N; r++){
            for(int c = 0; c < N; c++){
                T sumVal = T();
                for(int k = 0; k < N; k++){
                    sumVal += (*this)(r, k) * other(k, c);
                }
                result(r, c) = sumVal;
            }
        }
        return result;
    }

    // function to print out the matrix with nice alignment
    void print(const std::string& label = "") const {
        if(!label.empty()){
            std::cout << label << std::endl;
        }
        for(int r = 0; r < N; r++){
            for(int c = 0; c < N; c++){
                // adjust width for cleaner alignment as needed
                std::cout << std::setw(6) << (*this)(r,c) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // swap two rows in-place
    void swapRows(int r1, int r2) {
        if(r1 < 0 || r1 >= N || r2 < 0 || r2 >= N){
            std::cerr << "Row indices out of range. No swap performed." << std::endl;
            return;
        }
        for(int col = 0; col < N; col++){
            T temp = (*this)(r1, col);
            (*this)(r1, col) = (*this)(r2, col);
            (*this)(r2, col) = temp;
        }
    }

    // swap two columns in-place
    void swapCols(int c1, int c2) {
        if(c1 < 0 || c1 >= N || c2 < 0 || c2 >= N){
            std::cerr << "Column indices out of range. No swap performed." << std::endl;
            return;
        }
        for(int row = 0; row < N; row++){
            T temp = (*this)(row, c1);
            (*this)(row, c1) = (*this)(row, c2);
            (*this)(row, c2) = temp;
        }
    }

    // return sum of main diagonal + secondary diagonal
    T diagonalSum() const {
        T sumVal = T();
        for(int i = 0; i < N; i++){
            // main diagonal
            sumVal += (*this)(i, i);
            // secondary diagonal
            sumVal += (*this)(i, N-1 - i);
        }
        return sumVal;
    }
};

// templated function to update an element in the matrix
template <typename T>
void updateMatrixElement(Matrix<T>& mat, int row, int col, T newValue) {
    if(row < 0 || row >= mat.size() || col < 0 || col >= mat.size()) {
        std::cerr << "Invalid indices for update. No change made." << std::endl;
        return;
    }
    mat(row, col) = newValue;
}

// reading input and demonstration
int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream fin(filename);
    if(!fin.is_open()){
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }

    // file format:
    // First line: N  typeFlag
    // Followed by data for two NxN matrices
    // typeFlag = 0 => int
    // typeFlag = 1 => double

    int N;
    int typeFlag;
    fin >> N >> typeFlag;

    if(!fin.good() || N <= 0){
        std::cerr << "Invalid input in the first line or N <= 0." << std::endl;
        return 1;
    }

    // based on the typeFlag, we'll either read int or double
    if(typeFlag == 0){
        // Read two int matrices
        Matrix<int> A(N), B(N);
        for(int r = 0; r < N; r++){
            for(int c = 0; c < N; c++){
                fin >> A(r, c);
            }
        }
        for(int r = 0; r < N; r++){
            for(int c = 0; c < N; c++){
                fin >> B(r, c);
            }
        }

        // 1) Display both matrices
        A.print("1a) Matrix A:");
        B.print("1b) Matrix B:");

        // 2) A + B
        Matrix<int> sumAB = A + B;
        sumAB.print("2) A + B:");

        // 3) A * B
        Matrix<int> mulAB = A * B;
        mulAB.print("3) A * B:");

        // 4) Sum of main and secondary diagonals (example on A)
        int diagSum = A.diagonalSum();
        std::cout << "4) Sum of diagonals of A: " << diagSum << std::endl << std::endl;

        // 5) Swap rows (example: swap row 0 and row 1 of A)
        A.swapRows(0, 1);
        A.print("5) A after swapping rows 0 and 1:");

        // 6) Swap columns (example: swap col 2 and col 3 of A)
        A.swapCols(2, 3);
        A.print("6) A after swapping columns 2 and 3:");

        // 7) Update matrix rows/columns (example: set A(0,0) = 999)
        updateMatrixElement(A, 0, 0, 999);
        A.print("7) A after updating element (0,0) to 999:");

    } else {
        // read two double matrices
        Matrix<double> A(N), B(N);
        for(int r = 0; r < N; r++){
            for(int c = 0; c < N; c++){
                fin >> A(r, c);
            }
        }
        for(int r = 0; r < N; r++){
            for(int c = 0; c < N; c++){
                fin >> B(r, c);
            }
        }

        // 1) Display both matrices
        A.print("1a) Matrix A:");
        B.print("1b) Matrix B:");

        // 2) A + B
        Matrix<double> sumAB = A + B;
        sumAB.print("2) A + B:");

        // 3) A * B
        Matrix<double> mulAB = A * B;
        mulAB.print("3) A * B:");

        // 4) Sum of main and secondary diagonals (example on B)
        double diagSum = B.diagonalSum();
        std::cout << "4) Sum of diagonals of B: " << diagSum << std::endl << std::endl;

        // 5) Swap rows (example: swap row 0 and row 1 of B)
        B.swapRows(0, 1);
        B.print("5) B after swapping rows 0 and 1:");

        // 6) Swap columns (example: swap col 1 and col 2 of B)
        B.swapCols(1, 2);
        B.print("6) B after swapping columns 1 and 2:");

        // 7) Update matrix (example: set B(0,0) to 123.456)
        updateMatrixElement(B, 0, 0, 123.456);
        B.print("7) B after updating element (0,0) to 123.456:");
    }

    fin.close();
    return 0;
}
