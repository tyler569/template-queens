
#include <cstdio>
#include <type_traits>

// List Basics, Nil and Cons

struct Nil {};

// Forward declare Last for debug static assert
template <typename List> struct Last;

template <typename X, typename Xs>
struct Cons {
    // enforce correct construction for debugging
    static_assert( std::is_same<typename Last<Xs>::value, Nil>::value );
};

// First

template <typename List>
struct First {};

template <>
struct First<Nil> {
    using value = Nil;
};

template <typename X, typename Xs>
struct First<Cons<X, Xs>> {
    using value = X;
};

// Last
    // for debug static assert above only
    template <typename List>
    struct Last {
        using value = double; // to produce error in static assert above
    };

    template <>
    struct Last<Nil> {
        using value = Nil;
    };

    template <typename X, typename Xs>
    struct Last<Cons<X, Xs>> {
        using value = typename Last<Xs>::value;
    };

// List Concat

template <typename As, typename Bs>
struct ListConcat {
    using value = Cons<As, Bs>;
};

template <typename Bs>
struct ListConcat<Nil, Bs> {
    using value = Bs;
};

template <typename A, typename As, typename Bs>
struct ListConcat<Cons<A, As>, Bs> {
    using value = Cons<A, typename ListConcat<As, Bs>::value>;

    // static_assert(! std::is_same<value, Nil>::value);
};

/*template <typename A, typename As>
struct ListConcat<Cons<A, As>, Nil> {
    using value = Cons<A, As>;
};*/

// List Concat All

template <typename Ls>
struct ListConcatAll {};

template <>
struct ListConcatAll<Nil> {
    using value = Nil;
};

template <typename L1, typename Ls>
struct ListConcatAll<Cons<L1, Ls>> {
    using value = typename ListConcat<L1, typename ListConcatAll<Ls>::value>::value;

    // static_assert(std::is_convertible_v<L1, Ls>);
};

// Booleans

struct True {};
struct False {};

template <typename T>
struct Not {};

template <>
struct Not<True> {
    using value = False;
};

template <>
struct Not<False> {
    using value = True;
};

/*
// If

template <typename Bool, typename T, typename F>
struct If {};

template <typename T, typename F>
struct If<True, T, F> {
    using value = T;
};

template <typename T, typename F>
struct If<False, T, F> {
    using value = F;
};

// Is Same

template <typename A, typename B>
struct IsSame {
    using value = False;
};

template <typename A>
struct IsSame<A, A> {
    using value = True;
};
*/

// Or Else

// if V == Nil, then Else
template <typename V, typename Else>
struct OrElse {
    using value = V;
};

template <typename Else>
struct OrElse<Nil, Else> {
    using value = Else;
};

// Any True

template <typename List>
struct AnyTrue {};

template <>
struct AnyTrue<Nil> {
    using value = False;
};

template <typename More>
struct AnyTrue<Cons<True, More>> {
    using value = True;
};

template <typename More>
struct AnyTrue<Cons<False, More>> {
    using value = typename AnyTrue<More>::value;
};

// Boolean Or

template <typename A, typename B>
struct Or {};

template <>
struct Or<True, True> {
    using value = True;
};

template <>
struct Or<True, False> {
    using value = True;
};

template <>
struct Or<False, True> {
    using value = True;
};

template <>
struct Or<False, False> {
    using value = False;
};

// Natural Numbers (up to 8)

struct Z {};

template <typename N>
struct S {
    using value = S<N>;
};

using N0 = Z;
using N1 = S<N0>;
using N2 = S<N1>;
using N3 = S<N2>;
using N4 = S<N3>;
using N5 = S<N4>;
using N6 = S<N5>;
using N7 = S<N6>;
using N8 = S<N7>;
using N9 = S<N8>;
using N10 = S<N9>;
using N11 = S<N10>;
using N12 = S<N11>;

// Peano Equality

template <typename A, typename B> 
struct PeanoEqual {};

template <>
struct PeanoEqual<Z, Z> {
    using value = True;
};

template <typename X>
struct PeanoEqual<S<X>, Z> {
    using value = False;
};

template <typename X>
struct PeanoEqual<Z, S<X>> {
    using value = False;
};

template <typename X, typename Y>
struct PeanoEqual<S<X>, S<Y>> {
    using value = typename PeanoEqual<X, Y>::value;
};

// Peano LT

template <typename A, typename B> 
struct PeanoLT {};

template <>
struct PeanoLT<Z, Z> {
    using value = False;
};

template <typename X>
struct PeanoLT<S<X>, Z> {
    using value = False;
};

template <typename X>
struct PeanoLT<Z, S<X>> {
    using value = True;
};

template <typename X, typename Y>
struct PeanoLT<S<X>, S<Y>> {
    using value = typename PeanoLT<X, Y>::value;
};

// Peano Absolute Difference

template <typename A, typename B> 
struct PeanoAbsDiff {};

template <>
struct PeanoAbsDiff<Z, Z> {
    using value = Z;
};

template <typename X>
struct PeanoAbsDiff<S<X>, Z> {
    using value = S<X>;
};

template <typename X>
struct PeanoAbsDiff<Z, S<X>> {
    using value = S<X>;
};

template <typename X, typename Y>
struct PeanoAbsDiff<S<X>, S<Y>> {
    using value = typename PeanoAbsDiff<X, Y>::value;
};

// Range 0 - N

template <typename N>
struct Range {};

template <typename N>
struct Range<S<N>> {
    using value = typename ::Cons< N, typename Range<N>::value >;
};

template <>
struct Range<Z> {
    using value = Nil; // Cons<Z, Nil>; // - covered above
};

// Apply

template <typename Func, typename Arg>
struct Apply {};

template <typename Val>
struct Conj1 {};

template <typename List, typename Arg>
struct Apply<Conj1<List>, Arg> {
    using value = Cons<Arg, List>;
};

/*
template <typename List>
struct Apply<Conj1<List>, Nil> {
    using value = List; // maybe?
};
*/

// Map

template <typename Func, typename Xs>
struct Map {};

template <typename Func>
struct Map<Func, Nil> {
    using value = Nil;
};

template <typename Func, typename X, typename Xs>
struct Map<Func, Cons<X, Xs>> {
    using value = Cons<typename Apply<Func, X>::value, typename Map<Func, Xs>::value>;
};

template <typename Func>
struct Map1{};

template <typename Func, typename Xs>
struct Apply<Map1<Func>, Xs> {
    using value = typename ::Map<Func, Xs>::value;
};

// MapCat

template <typename Func, typename Xs>
struct MapCat {
    using chunks = typename Map<Func, Xs>::value;
    using value = typename ListConcatAll<chunks>::value;
};

template <typename Func>
struct MapCat<Func, Nil> {
    // ?
    // ? using value = typename ::Cons < typename Apply<Func, Nil>::value, Nil >; // ?
    // ?
    using value = Nil;
};

/*
template <typename Func, typename X, typename Xs>
struct MapCat<Func, Cons<X, Xs>> {
    // may be identical to above
    using map_x = typename Apply<Func, X>::value;
    using mapcat_rest = typename MapCat<Func, Xs>::value;
    using out = typename ::Cons< map_x, mapcat_rest >;
    // using value = applications;
    using value = out; // typename ::ListConcatAll<applications>::value;

    static_assert(! std::is_same<value, Nil>::value);
};
*/

// AppendIf

template <typename Pred, typename X, typename Ys>
struct AppendIf {};

template <typename X, typename Ys>
struct AppendIf<True, X, Ys> {
    using value = Cons<X, Ys>;
};

template <typename X, typename Ys>
struct AppendIf<False, X, Ys> {
    using value = Ys;
};

// Filter

template <typename Func, typename Xs>
struct Filter {};

template <typename Func>
struct Filter<Func, Nil> {
    using value = Nil;
};

template <typename Func, typename X, typename Xs>
struct Filter<Func, Cons<X, Xs>> {
    using pred = typename Apply<Func, X>::value;
    using filter_rest = typename Filter<Func, Xs>::value;
    using value = typename AppendIf < pred, X, filter_rest >::value;
};

// IsZero -- testing
/*
struct IsZero {};

template <typename Arg>
struct Apply<IsZero, Arg> {
    using value = False;
};

template <>
struct Apply<IsZero, Z> {
    using value = True;
};
*/

// Queens and partial application

template <typename X, typename Y>
struct Queen {};

template <typename X>
struct Queen1 {};

template <typename X, typename Y>
struct Apply<Queen1<X>, Y> {
    using value = typename ::Queen<X, Y>;
};

template <typename N, typename X>
struct QueensInRow {
    using value = typename Map<Queen1<X>, typename Range<N>::value>::value;
};

// Threatens

template <typename Q1, typename Q2>
struct Threatens {};

template <typename X1, typename Y1, typename X2, typename Y2>
struct Threatens<Queen<X1, Y1>, Queen<X2, Y2>> {

    using samerow = typename Or <
        typename PeanoEqual<X1, X2>::value,
        typename PeanoEqual<Y1, Y2>::value
    >::value;

    using samediag = typename PeanoEqual <
        typename PeanoAbsDiff<X1, X2>::value,
        typename PeanoAbsDiff<Y1, Y2>::value
    >::value;

    using value = typename Or <samerow, samediag>::value;
};

template <typename X1, typename Y1>
struct Threatens<Queen<X1, Y1>, Nil> {
    using value = False;
};


template <typename T>
struct Threatens1 {};

template <typename X, typename Y>
struct Apply<Threatens1<X>, Y> {
    using value = typename ::Threatens<X, Y>::value;
};

// Safe

template <typename Config, typename Queen>
struct Safe {
    using threats = typename Map < Threatens1<Queen>, Config >::value;
    using value = typename Not < typename AnyTrue<threats>::value >::value;
};

template <typename Config>
struct Safe1 {};

template <typename Config, typename Queen>
struct Apply<Safe1<Config>, Queen> {
    using value = typename ::Safe<Config, Queen>::value;
};

// Add Queen

template <typename N, typename X, typename C>
struct AddQueen {
    /* using value = typename OrElse < 
        typename Map < Conj1<C>,
            typename Filter < Safe1<C>,
                typename QueensInRow < N, X >::value
            >::value
        >::value,
        C
    >::value; */

    using row = typename QueensInRow < N, X >::value;
    using safe = typename Filter < Safe1<C>, row >::value;
    using value = typename Map < Conj1<C>, safe >::value;

    static_assert(! std::is_same<value, Cons<Nil, Nil>>::value);
};

template <typename N, typename X>
struct AddQueen2 {};

template <typename N, typename X, typename C>
struct Apply<AddQueen2<N, X>, C> {
    using value = typename ::AddQueen<N, X, C>::value;

    // static_assert(! std::is_same<C, Nil>::value);
};

// Add Queen To All

template <typename N, typename X, typename CS>
struct AddQueenToAll {
    using value = typename MapCat < AddQueen2<N, X>, CS >::value;

    static_assert(! std::is_same<value, Cons<Nil, Nil>>::value);
};

// Add Queens If

// forward declare AddQueens:
template <typename N, typename X, typename CS> struct AddQueens;

template <typename Pred, typename N, typename X, typename CS>
struct AddQueensIf {
    using added_to_all = typename AddQueenToAll < N, X, CS >::value;
    using value = typename ::AddQueens < N, S<X>, added_to_all >::value;
};

template <typename N, typename X, typename CS>
struct AddQueensIf<False, N, X, CS> {
    using value = CS;
};

// Add Queens

template <typename N, typename X, typename CS>
struct AddQueens {
    using can_add = typename PeanoLT<X, N>::value;
    using value = typename AddQueensIf < can_add, N, X, CS >::value;
};

// Solution:

template <typename N>
struct Solution {
    using all_values = typename AddQueens < N, Z, Cons<Nil, Nil> >::value;
    using value = typename ::First < all_values >::value;
};

// Utility snippets:

using Z1 = Cons<Z, Nil>;
using Z2 = Cons<Z, Z1>;
using Z3 = Cons<Z, Z2>;

using F1 = Cons<False, Nil>;
using F2 = Cons<False, F1>;
using F3 = Cons<False, F2>;

using Q00 = Queen<Z, Z>;
using Q20 = Queen<N2, Z>;
using Q22 = Queen<N2, N2>;
using Q23 = Queen<N2, N3>;

using Nums = Cons<Z, Cons<N2, Cons<N1, Cons<Z, Cons<N1, Nil>>>>>;

// Tests:

using legalCompare = PeanoEqual<S<Z>, S<Z>>::value;
using testRange = Range<N2>::value;
using illegalCompare = PeanoEqual<True, Cons<Z, False>>;
using cat = ListConcat<Z1, Z1>::value;
using testAnyF = AnyTrue<F3>::value;
using testAnyT = AnyTrue<Cons<True, F2>>::value;
using catAll = ListConcatAll<Cons<Z1, Cons<Z2, Nil>>>::value;
// using conj = Apply<Conj1<Z>, Cons<N1, Nil>>::value;
using conj = Apply<Conj1<Cons<N1, Nil>>, Z>::value;
// using mapConj = Map<Conj1<True>, Cons<Z1, Cons<Z1, Nil>>>::value;
// using filter0 = Filter<IsZero, Nums>::value;
using queens = QueensInRow<N2, N2>::value;
using threatensRow = Threatens<Q00, Q20>::value;
using threatensDiag = Threatens<Q00, Q22>::value;
using threatensNot = Threatens<Q00, Q23>::value;

using mapConj = Map<Conj1<Cons<Z, Nil>>, Nil>::value;

using safeTest = Safe<Nil, Queen<Z, Z>>::value;

// using addQueen = AddQueen<N2, Z, Nil>::value;
// using addQueenR2 = Map<AddQueen2<N1, N2>, addQueen>::value;

// using addQueenS = AddQueen<N1, N2, Cons<Queen<Z, Z>, Nil>>::value;

// using mcAddQueen = MapCat<AddQueen2<Z, N2>, Cons<Cons<Queen<N1, N1>, Nil>, Nil>>::value;

// using test = typename MapCat < AddQueen2<N4, Z>, Cons<Nil, Nil> >::value;

using solved = Solution<N8>::value;

template <typename T>
int f(T) {
    #if defined(__GNUC__)
        puts(__PRETTY_FUNCTION__);
    #elif defined(__MSVC__)
        puts(__FUNCSIG__);
    #endif
    return 0;
}

int main() {
    /*
    f(legalCompare{});      // True
    f(testRange{});         // Cons<S<S<Z>>, Cons<S<Z>, Cons<Z, Nil>>>
    // f(illegalCompare{});    // PeanoEqual<True, Cons<Z, False>>   "a runtime error"
    f(cat{});               // Cons<Z, Cons<Z, Nil>>
    f(testAnyF{});          // False
    f(testAnyT{});          // True
    f(catAll{});            // Cons<Z, Cons<Z, Cons<Z, Nil>>>
    f(conj{});              // Cons<Z, Cons<S<Z>, Nil>>
    // f(mapConj{});           // Cons<Cons<True, Cons<Z, Nil>>, Cons<Cons<True, Cons<Z, Nil>>, Nil>>
    // f(filter0{});           // Cons<Z, Cons<Z, Nil>>
    f(queens{});            // Cons<Queen<S<S<Z>>, S<S<Z>>>, Cons<Queen<S<S<Z>>, S<Z>>, Cons<Queen<S<S<Z>>, Z>, Nil>>>
    f(threatensRow{});      // True
    f(threatensDiag{});     // True
    f(threatensNot{});      // False
    f(mapConj{});           // 
    f(safeTest{});          // True

    //f(addQueen{});          // 
    //f(addQueenS{});         //
    //f(addQueenR2{});        //
    */
    //f(test{});

    f(solved{});
}

