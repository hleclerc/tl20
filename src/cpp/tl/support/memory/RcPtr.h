#pragma once

#include "../Displayer.h"

BEG_TL_NAMESPACE

/**
 * T must have an accessible attribute ref_count, starting at 0
 */
template<class T>
struct RcPtr {
    T_U         RcPtr        ( const RcPtr<U> &obj );
    /**/        RcPtr        ( const RcPtr &obj );
    T_U         RcPtr        ( RcPtr<U> &&obj );
    /**/        RcPtr        ( RcPtr &&obj );
    T_U         RcPtr        ( U *obj );
    /**/        RcPtr        ();

    /**/       ~RcPtr        ();
 
    T_U static  RcPtr        from_ptr   ( U *ptr );
 
    T*          operator->   () const;
    T&          operator*    () const;
    T*          get          () const;
 
    //void      display      ( Displayer &sr ) const { sr.append_pointer( , , const std::function<void ()> &cb)dd_ptr( data ); }
    explicit    operator bool() const;
 
    T_U RcPtr&  operator=    ( const RcPtr<U> &obj );
    RcPtr&      operator=    ( const RcPtr &obj );
    T_U RcPtr&  operator=    ( RcPtr<U> &&obj );
    RcPtr&      operator=    ( RcPtr &&obj );
   
    bool        operator==   ( const RcPtr<T> &p ) const;
    bool        operator!=   ( const RcPtr<T> &p ) const;
    bool        operator<    ( const RcPtr<T> &p ) const; 
    bool        operator<=   ( const RcPtr<T> &p ) const;
    bool        operator>    ( const RcPtr<T> &p ) const;
    bool        operator>=   ( const RcPtr<T> &p ) const;
   
    void        clear        ();

    static void inc_ref      ( T *data );
    static void dec_ref      ( T *data );
  
    void        inc_ref      ();
    void        dec_ref      ();
  
    T*          data;        ///<
};

T_T Str pointer_repr( const RcPtr<T> &ptr ) { return pointer_repr( ptr.data ); }

END_TL_NAMESPACE

#include "RcPtr.cxx" // IWYU pragma: export
