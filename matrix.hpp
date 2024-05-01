#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include<vector>
#include <array>
#include <cassert>
#include <iostream>
#include <iomanip>



namespace algebra{


    enum class StorageOrder{
        row_wise,
        column_wise
    };

    using position = std::array<std::size_t,2>;

    template<StorageOrder S>
    struct custom_comparer
    {
        bool operator()(const position& left, const position& right) const
        {   
            if constexpr(S == StorageOrder::row_wise)
                return left < right;   
            return left[1] < right[1] or (left[1] == right[1] and left[0] < right[0]);
        }
    };

    template<class T, StorageOrder S>
    using dynamic_container = std::map<position,T,custom_comparer<S>>;

    using idx_type = long unsigned int;



    // struct were I store all the data for the compressed version of the matrix
    template< class T, StorageOrder S >
    struct Compressed_struct{

        std::vector<T> values;
        std::vector<idx_type> inner_idx; //in row_wise inner_idx = row_idx
        std::vector<idx_type> outer_idx; //in row_wise outer_idx = col_idx

        //method to clear containers
        inline void clear(){
            values.clear();
            inner_idx.clear();
            outer_idx.clear();
        }

        // switch i and j in colummn_wise order
        inline void adjust_idx(idx_type& in, idx_type& out){
            if(S==StorageOrder::column_wise)
                std::swap(in,out);
        }
    };





// matrix class

    template<class T,StorageOrder S>
    class Matrix{

    private:
        dynamic_container<T,S> m_dyn_data;
        Compressed_struct<T,S> m_compr_data;

        bool m_is_compr = false;

        idx_type m_nnz = 0;
        idx_type m_nrows = 0;
        idx_type m_ncol = 0;


        // useful boolean method
        inline bool is_row_wise() const{
            return S == StorageOrder::row_wise;
        }

        inline bool is_compressed() const{
            return m_is_compr;
        }

        inline bool is_in_range(std::size_t i, std::size_t j)const{
            return i >= 0 and i < m_nrows and j>= 0 and j < m_ncol;
        }


    public:
        

        //constructor
        Matrix(idx_type row, idx_type col) : 
        m_nrows(row), m_ncol(col) {};

        // resize method
        void resize(idx_type row,idx_type col);
        // call operators:

        // add a new element in position (i,j)
        T& operator ()(std::size_t i, std::size_t j);

        // read the element in position (i,j)
        T& operator ()(std::size_t i, std::size_t j)const;

        // change dynamic/compress storage
        void compress();
        void uncompress();
        
        // print operator
        template <class U, StorageOrder O>
        friend std::ostream &
        operator<<(std::ostream &stream, Matrix<U,O> &M);
/*
        // check compatible size
        template <class U, StorageOrder O>
        friend std::vector<U> check_compatible_sizes (const Matrix<U,O> &M,const std::vector<U>& v){

        };
        */

        // performing Av = b
        template <class U, StorageOrder O>
        friend std::vector<U> operator* (const Matrix<U,O> &M,const std::vector<U>& v);
    

    };


}


#include "matrix_impl.hpp"

#endif