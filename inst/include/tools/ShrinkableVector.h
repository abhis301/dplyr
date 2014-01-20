#ifndef dplyr_ShrinkableVector_H
#define dplyr_ShrinkableVector_H

namespace Rcpp {
    
    template <int RTYPE>
    class ShrinkableVector {
    public:
        typedef typename traits::storage_type<RTYPE>::type STORAGE ;
        
        ShrinkableVector( int n, SEXP origin ) : 
          data( no_init(n) ), max_size(n), start( internal::r_vector_start<RTYPE>(data) ) 
        {
            SET_ATTRIB( data, ATTRIB(origin) );
            SET_OBJECT( data, OBJECT(origin) ) ;
        }
        
        inline void resize( int n){
            SETLENGTH( data, n ) ;    
        }
        
        inline operator SEXP() const {
            return data ;
        }
        
        inline void borrow(const SlicingIndex& indices, STORAGE* begin){
            int n = indices.size() ;
            for( int i=0; i<n ; i++){
                start[i] = begin[indices[i]] ;    
            }
            // memcpy( start, begin, n * sizeof(STORAGE) );
            SETLENGTH(data, n) ;
        }
        
        ~ShrinkableVector(){
            // restore the initial length so that R can reclaim the memory
            SETLENGTH( data, max_size );   
        }
        
    private:
        Rcpp::Vector<RTYPE> data ;
        int max_size ;
        STORAGE* start ;
    } ;
    
}

#endif
