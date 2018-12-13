/********************************************************
*   By Euxhen Hasanaj                                   *
*   Linear time sorting algorithms                      *
*                                                       *
*   Bucket Sort & Radix Sort                            *
********************************************************/

#include <iostream>
#include <vector>
#include <limits>
#include <cassert>

template<class T>
void sort_indices(const std::vector<T>& sequence,
                        std::vector<int>& sorted_indices) {
    // Needed by bucket sort

    /* Given a vector, stores the indices which represent the sorted
    order of elements into sorted_indices. */
    // limits
    T m = std::numeric_limits<T>::max(); // min
    T M = std::numeric_limits<T>::min(); // max
    int len = sequence.size();
    // get min and max elements in sequence
    // needed for normalization
    for (T x : sequence) {
        m = std::min(x, m);
        M = std::max(x, M);
    }
    // initialize
    T N = M - m + 1;
    std::vector<std::vector<int>> indices(N, std::vector<int>(0, 0));
    // push indices to index vector
    for (int i = 0; i < len; ++i)
        indices[sequence[i] - m].push_back(i);
    // construct vector of sorted indices

    sorted_indices.clear();
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < indices[i].size(); ++j)
            sorted_indices.push_back(indices[i][j]);
}

template<class T>
void bucket_sort(std::vector<T>& sequence) {
    /* Sort given vector in O(n) time provided that the entries are
    integers. Calls sort_indices and constructs sorted  vector.  */
    int len = sequence.size();

    std::vector<int> sorted_indices;
    sort_indices(sequence, sorted_indices);

    // construct sorted array
    std::vector<T> temp(len); 
    for (int i = 0; i < len; ++i) // build temporary sorted vector 
        temp[i] = sequence[sorted_indices[i]];
    for (int i = 0; i < len; ++i) // copy to original vector
        sequence[i] = temp[i];
}

template<class T>
void bucket_sort(T sequence[], int len) {
    /* Bucket sort for array like structures */
    /* Parameters: 1D array, array size */
    std::vector<T> temp(len);
    for (int i = 0; i < len; ++i)	// copy array to vector
        temp[i] = sequence[i];
    bucket_sort(temp);				// sort vector
    for (int i = 0; i < len; ++i)	// copy vector back to array
        sequence[i] = temp[i];
}

template<class T>
void sort_mapped_indices(const std::vector<std::vector<T>>& sequence,
                                    std::vector<int>& index_map,
                                    int column) {
    // Needed by radix_sort

    /* Similar to sort_indices, but here we are given a 2D vector and
    we limit our bucket list to the entries specified  by  column. An
    index map is given which specifies the order that we read off the
    rows of sequence (since we want to preserve the sorting  done  by
    the previous calls to this function). For more google radix sort.*/
    T m = std::numeric_limits<T>::max(); // min
    T M = std::numeric_limits<T>::min(); // max
    int len = sequence.size();
    // min and max elements
    // needed for normalization
    for (int i = 0; i < len; ++i) {
        assert(column < sequence[i].size()); // ensure no short columns appear

        m = std::min(sequence[i][column], m);
        M = std::max(sequence[i][column], M);
    }
    // initialize
    T N = M - m + 1;
    std::vector<std::vector<int>> indices(N, std::vector<int>(0, 0));
    // push indices by following the order specified in index_map
    for (int i : index_map)
        indices[sequence[i][column] - m].push_back(i);
    // reconstruct index_map with the newly sorted indices
    index_map.clear();
    for (int i = 0; i < N; ++i)
            for (int j = 0; j < indices[i].size(); ++j)
                    index_map.push_back(indices[i][j]);
    assert(index_map.size() == len);// index_map should have as many indices
                                                        // as there are rows
}

template<class T>
void radix_sort(std::vector<std::vector<T>>& sequence) {
    /* Applies radix sort to given 2D vector in O(n) time. */
    int rows = sequence.size(); // number of rows
    int row_length = sequence[0].size(); // row length

    std::vector<int> index_map(rows); // order of rows at any given time
    for (int i = 0; i < rows; ++i)
        index_map[i] = i; // default ordering
    // Update index_map for every column of sequence starting with
    // the last column. Google radix sort.
    for (int i = row_length - 1; i >= 0; --i)
        sort_mapped_indices(sequence, index_map, i);
    // Temporary vector holding the final sorted sequence
    std::vector<std::vector<T>> result;
    for (int i : index_map)
        result.push_back(sequence[i]);
    sequence = result; // change pointer value
}

template<class T, size_t d1, size_t d2>
void radix_sort(T (&sequence)[d1][d2], int dim1, int dim2) {
    /* Radix sort for 2D array like structure */
    /* Parameters: 2D array, number of rows, number of columns per row */
    std::vector<std::vector<T>> temp(dim1, std::vector<T>(dim2));
    for (int i = 0; i < dim1; ++i)		// copy array to vector
        for (int j = 0; j < dim2; ++j)
            temp[i][j] = sequence[i][j];
    radix_sort(temp);					// sort vector
    for (int i = 0; i < dim1; ++i)		// copy vector back to array
        for (int j = 0; j < dim2; ++j)
            sequence[i][j] = temp[i][j];
}
