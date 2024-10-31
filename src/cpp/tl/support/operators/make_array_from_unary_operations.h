#pragma once

#include "../containers/StaticSizesOf.h"
#include "../containers/ArrayTypeFor.h"
#include "../containers/ItemTypeOf.h"

#include "../common_ctor_selectors.h"

BEG_TL_NAMESPACE

auto make_array_from_unary_operations( auto &&functor, auto &&a ) {
    using Da = DECAYED_TYPE_OF( a );

    using Sa = StaticSizesOf<Da>::value;
    using Sr = Sa;

    constexpr auto na = Sa::size;

    using Ia = ItemTypeOf<Da>::value;
    using Ir = DECAYED_TYPE_OF( functor( *(const Ia *)nullptr ) );

    using ArrayType = ArrayTypeFor<Ir,Sr,1>::value;

    return ArrayType( FromOperationOnItemsOf(), FORWARD( functor ), PrimitiveCtIntList<na>(), FORWARD( a ) );
}

END_TL_NAMESPACE
