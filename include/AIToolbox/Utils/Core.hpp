#ifndef AI_TOOLBOX_UTILS_CORE_HEADER_FILE
#define AI_TOOLBOX_UTILS_CORE_HEADER_FILE

#include <cstddef>
#include <cmath>
#include <limits>
#include <algorithm>

#include <AIToolbox/Types.hpp>

namespace AIToolbox {
    /**
     * @brief Copies a 3d container into another 3d container.
     *
     * The containers needs to support data access through
     * operator[]. In addition, the dimensions of the
     * containers must match the ones specified.
     *
     * This is important, as this function DOES NOT perform
     * any size checks on the containers.
     *
     * @tparam T Type of the input container.
     * @tparam U Type of the output container.
     * @param in Input container.
     * @param out Output container.
     * @param d1 First dimension of the containers.
     * @param d2 Second dimension of the containers.
     * @param d3 Third dimension of the containers.
     */
    template <typename T, typename U>
    void copyTable3D(const T & in, U & out, const size_t d1, const size_t d2, const size_t d3) {
        for ( size_t i = 0; i < d1; ++i )
            for ( size_t j = 0; j < d2; ++j )
                for ( size_t x = 0; x < d3; ++x )
                    out[i][j][x] = in[i][j][x];
    }

    /**
     * @brief This function checks if two doubles near [0,1] are reasonably equal.
     *
     * If the numbers are not near [0,1], the result is not guaranteed to be
     * what may be expected. The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are close enough, false otherwise.
     */
    inline bool checkEqualSmall(const double a, const double b) {
        return ( std::fabs(a - b) <= 5 * std::numeric_limits<double>::epsilon() );
    }

    /**
     * @brief This function checks if two doubles near [0,1] are reasonably different.
     *
     * If the numbers are not near [0,1], the result is not guaranteed to be
     * what may be expected. The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are far away enough, false otherwise.
     */
    inline bool checkDifferentSmall(const double a, const double b) {
        return !checkEqualSmall(a,b);
    }

    /**
     * @brief This function checks if two doubles are reasonably equal.
     *
     * The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are close enough, false otherwise.
     */
    inline bool checkEqualGeneral(const double a, const double b) {
        if ( checkEqualSmall(a,b) ) return true;
        return ( std::fabs(a - b) / std::min(std::fabs(a), std::fabs(b)) < std::numeric_limits<double>::epsilon() );
    }

    /**
     * @brief This function checks if two doubles are reasonably different.
     *
     * The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are far away enough, false otherwise.
     */
    inline bool checkDifferentGeneral(const double a, const double b) {
        return !checkEqualGeneral(a,b);
    }

    /**
     * @brief This function compares two AIToolbox::Vectors of equal size.
     *
     * @param lhs The left hand size of the comparison.
     * @param rhs The right hand size of the comparison.
     *
     * @return 1 if the lhs is greater than the rhs, 0 if they are equal, -1 otherwise.
     */
    template <typename V>
    int veccmp(const V & lhs, const V & rhs) {
        assert(lhs.size() == rhs.size());
        for (decltype(lhs.size()) i = 0; i < lhs.size(); ++i) {
            if (lhs[i] > rhs[i]) return 1;
            if (lhs[i] < rhs[i]) return -1;
        }
        return 0;
    }

    inline bool operator<(const Vector & lhs, const Vector & rhs) {
        return veccmp(lhs, rhs) < 0;
    }

    inline bool operator>(const Vector & lhs, const Vector & rhs) {
        return veccmp(lhs, rhs) > 0;
    }

    /**
     * @brief This function returns whether a sorted range contains a given element, via sequential scan.
     *
     * The idea behind this function is that for small sorted vectors it is
     * faster to do a sequential scan rather than employing the heavy handed
     * technique of binary search.
     *
     * @tparam V The type of the vector to be scanned.
     * @param v The vector to be scanned.
     * @param elem The element to be looked for in the vector.
     *
     * @return True if the vector contains that element, false otherwise.
     */
    template <typename V>
    bool sequential_sorted_contains(const V & v, decltype(v[0]) elem) {
        // Maybe this could be done with iterators?
        for (const auto & e : v) {
            if (e < elem) continue;
            if (e == elem) return true;
            return false;
        }
        return false;
    }

    namespace Impl {
        template <typename Iterator, typename Check = void>
        struct BaseIter {
            using BaseIterator = Iterator;
            Iterator operator()(Iterator it) { return it; }
        };

        template <typename Iterator>
        struct BaseIter<Iterator, decltype(std::declval<Iterator*>()->base(), void())> {
            using BaseIterator = decltype(std::declval<Iterator>().base());
            BaseIterator operator()(const Iterator & it) { return it.base(); }
        };
    }

    /**
     * @brief This function returns the base iterator for any given iterator.
     *
     * A base iterator exists if the iterator implements the method base(). If
     * not, a copy of the iterator itself is returned.
     *
     * @param it The iterator to return the base of.
     *
     * @return The base iterator of the input.
     */
    template <typename Iterator>
    typename Impl::BaseIter<typename std::remove_reference<Iterator>::type>::BaseIterator baseIter(Iterator && it) {
        return Impl::BaseIter<typename std::remove_reference<Iterator>::type>()(std::forward<Iterator>(it));
    }
}

#endif
