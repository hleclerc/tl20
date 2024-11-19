#pragma once

#ifdef SDOT_DEBUG_MODE
    #define STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( RET, COND, MSG ) do { static_assert( COND, MSG ); return RET; } while( false )
    #define STATIC_ASSERT_IN_IF_CONSTEXPR( COND, MSG ) static_assert( COND, MSG )
#else
    #define STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( RET, COND, MSG ) return RET
    #define STATIC_ASSERT_IN_IF_CONSTEXPR( COND, MSG ) 
#endif
