/********************************************************
*   By Euxhen Hasanaj                                   *
*   Selection algorithms                                *
********************************************************/

/********************************************************
 * Contains quick_select and deterministic_select       *
 * methods for finding the k-th smallest  element       *
 * in a given sequence of  numbers.  quick_select       *
 * relies on a custom choice of the pivot,  while       *
 * deterministic_select chooses a  pivot  out  of       *
 * medians of n/5 sorted groups of size 5.              *
 *                                                      *
 * quick_select runs in O(n) expected time              *
 * deterministic_select runs in O(n) deterministic time *
 * *****************************************************/

#include <iostream>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <stdio.h>

int median(int a, int b, int c) {
    // Calculates median of three elements
    return std::max(std::min(a, b), std::min(std::max(a, b), c));
}

template<class T>
void release(std::vector<T>& v) {
    // free up vector
    v.clear();
    std::vector<T>(v).swap(v);
}

template<class T>
T choose_pivot(const std::vector<T>& seq) {
    /* Computes pivot as median of the median of three groups each of
    three equally spaced points in sequence. */
    int low = 0, high = seq.size() - 1;
    int step = (high - low + 1) / 9;
    // Compute three medians among 9 equally spaced points
    T median1 = median(seq[low], seq[low + step], seq[low + 2*step]);
    T median2 = median(seq[low + 3*step], seq[low + 4*step], seq[low + 5*step]);
    T median3 = median(seq[low + 6*step], seq[low + 7*step], seq[high]);
    // Take median of medians
    T pivot = median(median1, median2, median3);
}

template<class T>
T quick_select(std::vector<T>& sequence, int k) {
    /* Find the k-th smallest element in the given array.
    Expected running time is O(n) where n is the size of the sequence
    given and assuming the values are randomly distributed. Choose  a
    pivot and break down sequence into parts, then choose the part to
    continue operating on. */

    // input error
    if (k < 0 || k > sequence.size()) {
        std::cout<<"In quick_select, index out of bounds.\n";
        std::cout<<"Exiting...\n";
        exit(EXIT_FAILURE);
    }
    if (sequence.size() == 1)	return sequence[0];

    std::vector<int> smaller, equal, greater;
    // choose pivot
    T pivot = choose_pivot(sequence);

    // split sequence into three groups
    for (T x : sequence) {
        if (x < pivot)              smaller.push_back(x);
        else if (x == pivot)        equal.push_back(x);
        else                        greater.push_back(x);
    }
    int s = smaller.size(), e = equal.size(), g = greater.size();
    // recur accordingly
    if (k <= s) {
        release(equal), release(greater); // free up vectors
        return quick_select(smaller, k);
    } else if (k <= s + e) {
        return pivot;
    } else {
        release(smaller), release(equal); // free up vectors
        return quick_select(greater, k - s - e);
    }
}

template<class T>
void find_baby_medians(std::vector<T>& seq, std::vector<T>& medians) {
    /* Divide seq into groups of length 5, sort each  group  and  take 
     * the median for each group. Our pivot will be the median of these 
     * baby medians*/
    int groups = std::ceil((double)seq.size() / 5), i;
    for (i = 0; i < groups - 1; ++i) { // all full blocks
        std::sort(seq.begin() + i*5, seq.begin() + (i+1) * 5);  // sort segment
        medians.push_back(seq[i*5 + 2]);   // middle element out of 5
    }
    if (seq.size() % 5 != 0) { // incomplete last block
        std::sort(seq.begin() + i*5, seq.end());
        medians.push_back(seq[i*5 + (seq.size() % 5) / 2]); // mid element
    } else { // full last block
        std::sort(seq.begin() + i*5, seq.end());
        medians.push_back(seq[i*5 + 2]);
    }
}

template<class T>
T deterministic_select(std::vector<T>& sequence, int k) {
    /* Find the k-th smallest element in the given array.
    Deterministic running time is O(n) where n is the size of the se-
    quence given and assuming the  values  are  randomly distributed.*/

    // input error
    if (k < 0 || k > sequence.size()) {
        std::cout<<"In deterministic_select, index out of bounds.\n";
        std::cout<<"Exiting...\n";
        exit(EXIT_FAILURE);
    }

    if (sequence.size() == 1)	return sequence[0];

    std::vector<T> smaller, equal, greater;

    std::vector<T> baby_medians; // medians for each group
    find_baby_medians(sequence, baby_medians);
    // choose pivot using the deterministic method
    int pivot = deterministic_select(baby_medians, 
                            std::ceil((double) baby_medians.size() / 2));

    // divide sequence into three groups
    for (T x : sequence) {
        if (x < pivot)		smaller.push_back(x);
        else if (x == pivot)	equal.push_back(x);
        else		        greater.push_back(x);
    }
    int s = smaller.size(), e = equal.size(), g = greater.size();
    // recur accordingly
	if (k <= s) {
            release(equal), release(greater); // free up vectors
            return deterministic_select(smaller, k);
	} else if (k <= s + e) {
            return pivot;
	} else {
            release(smaller), release(equal); // free up vectors
            return deterministic_select(greater, k - s - e);
	}
}
