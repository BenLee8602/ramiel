#include <type_traits>
#include <utility>

namespace ramiel {


    //////////////////////////////// REVERSE TYPE LIST ////////////////////////////////

    template<typename T, typename... Ts>
    struct ReverseTs;

    template<typename... Ts, typename... Us>
    constexpr ReverseTs<Ts..., Us...> cat(ReverseTs<Ts...>, ReverseTs<Us...>);

    template<typename T>
    struct ReverseTs<T> {
        using Type = ReverseTs<T>;
    };

    template<typename T, typename... Ts>
    struct ReverseTs {
        using Type = decltype(cat(
            typename ReverseTs<Ts...>::Type{},
            ReverseTs<T>{}
        ));
    };


    //////////////////////////////// TUPLE ////////////////////////////////

    template<typename T, typename... Ts>
    struct TupleImpl;

    template<typename... Ts>
    TupleImpl<Ts...> tupleFromReverseTs(ReverseTs<Ts...>);

    template<typename T>
    struct TupleImpl<T> {
        T val;

        template<size_t N>
        T& get() {
            static_assert(N == 0, "tuple index out of bounds");
            return val;
        }

        template<size_t N>
        const T& get() const {
            static_assert(N == 0, "tuple index out of bounds");
            return val;
        }
    };

    template<typename T, typename... Ts>
    struct TupleImpl : private TupleImpl<Ts...> {
        T val;
        
        template<size_t N>
        auto& get() {
            if constexpr (N == 0) return val;
            else return TupleImpl<Ts...>::template get<N - 1>();
        }
        
        template<size_t N>
        const auto& get() const {
            if constexpr (N == 0) return val;
            else return TupleImpl<Ts...>::template get<N - 1>();
        }
    };


    template<typename... Ts>
    struct Tuple {
        static constexpr size_t size = sizeof...(Ts);

        using Reversed_ts = typename ReverseTs<Ts...>::Type;
        using TupleImpl_t = decltype(tupleFromReverseTs(Reversed_ts{}));
        TupleImpl_t tuple;

        template<size_t N>
        auto& get() {
            static_assert(N < size, "tuple index out of range");
            return tuple.template get<size - N - 1>();
        }

        template<size_t N>
        const auto& get() const {
            static_assert(N < size, "tuple index out of range");
            return tuple.template get<size - N - 1>();
        }
    };


    template<size_t... Ns, typename... Ts>
    Tuple<Ts...> makeTupleImpl(std::index_sequence<Ns...>, const Ts&... args) {
        Tuple<Ts...> tuple;
        ((tuple.template get<Ns>() = args), ...);
        return tuple;
    }

    template<typename... Ts>
    Tuple<Ts...> makeTuple(const Ts&... args) {
        return makeTupleImpl(std::make_index_sequence<sizeof...(Ts)>{}, args...);
    }


    //////////////////////////////// TUPLE ARITHMETIC ////////////////////////////////

    template<size_t... Ns, typename... Ts>
    Tuple<Ts...> addTupleImpl(std::index_sequence<Ns...>, const Tuple<Ts...>& l, const Tuple<Ts...>& r) {
        Tuple<Ts...> res;
        ((res.template get<Ns>() = l.template get<Ns>() + r.template get<Ns>()), ...);
        return res;
    }

    template<size_t... Ns, typename... Ts>
    Tuple<Ts...> subtractTupleImpl(std::index_sequence<Ns...>, const Tuple<Ts...>& l, const Tuple<Ts...>& r) {
        Tuple<Ts...> res;
        ((res.template get<Ns>() = l.template get<Ns>() - r.template get<Ns>()), ...);
        return res;
    }

    template<typename U, size_t... Ns, typename... Ts>
    Tuple<Ts...> scaleTupleImpl(std::index_sequence<Ns...>, const Tuple<Ts...>& l, const U& r) {
        Tuple<Ts...> res;
        ((res.template get<Ns>() = l.template get<Ns>() * r), ...);
        return res;
    }


    template<typename... Ts>
    Tuple<Ts...> operator+(const Tuple<Ts...>& l, const Tuple<Ts...>& r) {
        return addTupleImpl(std::make_index_sequence<sizeof...(Ts)>{}, l, r);
    }

    template<typename... Ts>
    Tuple<Ts...> operator-(const Tuple<Ts...>& l, const Tuple<Ts...>& r) {
        return subtractTupleImpl(std::make_index_sequence<sizeof...(Ts)>{}, l, r);
    }

    template<typename U, typename... Ts>
    Tuple<Ts...> operator*(const Tuple<Ts...>& l, const U& r) {
        return scaleTupleImpl(std::make_index_sequence<sizeof...(Ts)>{}, l, r);
    }

    template<typename U, typename... Ts>
    Tuple<Ts...> operator/(const Tuple<Ts...>& l, const U& r) {
        return scaleTupleImpl(std::make_index_sequence<sizeof...(Ts)>{}, l, (U)1 / r);
    }


    //////////////////////////////// REFLECTION ////////////////////////////////

    // my version of the great c++ type loophole!
    // read more about it here:
    // blog: https://alexpolt.github.io/type-loophole.html
    // github: https://github.com/alexpolt/luple/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
    template<class T, int N>
    struct FieldIndex {
        friend auto getFieldType(FieldIndex<T, N>);
    };
#pragma GCC diagnostic pop

    template<class T, typename U, int N>
    struct GetFieldType_t {
        friend auto getFieldType(FieldIndex<T, N>) { return U{}; };
    };

    template<class T, size_t N>
    struct TypeDetector {
        template<typename U, size_t = sizeof(GetFieldType_t<T, U, N>)>
        operator U();
    };

    template<class T, size_t... Ns>
    constexpr size_t getFieldCount(...) {
        return sizeof...(Ns) - 1;
    }

    template<class T, size_t... Ns>
    constexpr decltype(T{ TypeDetector<T, Ns>{}... }, 0) getFieldCount(size_t) {
        static_assert(
            std::is_aggregate<T>::value,
            "cant create tuple from non-aggregate type"
        );
        return getFieldCount<T, Ns..., sizeof...(Ns)>(0);
    }

    template<class T, size_t... Ns>
    constexpr Tuple<decltype(getFieldType(FieldIndex<T, Ns>{}))...>
    asTupleImpl(std::index_sequence<Ns...>);

    template<class T>
    using AsTuple = decltype(asTupleImpl<T>(
        std::make_index_sequence<getFieldCount<T>(0)>{}
    ));
    
}
