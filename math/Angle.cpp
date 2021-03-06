// Copyright (c) 2015-2016 Josh Blum
//                    2020 Nicholas Corgan
// SPDX-License-Identifier: BSL-1.0

#include <Pothos/Framework.hpp>
#include <cstdint>
#include <iostream>
#include <complex>
#include <algorithm> //min/max
#include <type_traits>
#include "FxptHelpers.hpp"

//
// Implementation getters to be called on class construction
//

template <typename InType, typename OutType>
using AngleFcn = void(*)(const InType*, OutType*, const size_t);

template <typename InType, typename OutType>
static inline AngleFcn<InType, OutType> getAngleFcn()
{
    return [](const InType* in, OutType* out, const size_t num)
    {
        for (size_t i = 0; i < num; ++i) out[i] = getAngle(in[i]);
    };
}

/***********************************************************************
 * |PothosDoc Angle
 *
 * Compute the angle of every complex input element.
 *
 * out[n] = atan2(Re{in[n]}, Im{in[n]})
 *
 * |category /Math
 * |keywords math angle complex arg atan
 *
 * |param dtype[Data Type] The input data type.
 * The output type is always real.
 * The floating point outputs are in radians between -pi and +pi.
 * The fixed point outputs use a signed 16-bit range to represent -pi
 * through +pi (non-inclusive).
 * |widget DTypeChooser(cfloat=1,cint=1,dim=1)
 * |default "complex_float32"
 * |preview disable
 *
 * |factory /comms/angle(dtype)
 **********************************************************************/
template <typename InType, typename OutType>
class Angle : public Pothos::Block
{
public:
    Angle(const size_t dimension): _fcn(getAngleFcn<InType, OutType>())
    {
        this->setupInput(0, Pothos::DType(typeid(InType), dimension));
        this->setupOutput(0, Pothos::DType(typeid(OutType), dimension));
    }

    void work(void)
    {
        //number of elements to work with
        auto elems = this->workInfo().minElements;
        if (elems == 0) return;

        //get pointers to in and out buffer
        auto inPort = this->input(0);
        auto outPort = this->output(0);
        const InType *in = inPort->buffer();
        OutType *out = outPort->buffer();

        //compute angle using templated function
        const size_t N = elems*inPort->dtype().dimension();
        _fcn(in, out, N);

        //produce and consume on 0th ports
        inPort->consume(elems);
        outPort->produce(elems);
    }

private:
    AngleFcn<InType, OutType> _fcn;
};

/***********************************************************************
 * registration
 **********************************************************************/
static Pothos::Block *angleFactory(const Pothos::DType &dtype)
{
    #define ifTypeDeclareFactory_(intype, outtype) \
        if (Pothos::DType::fromDType(dtype, 1) == Pothos::DType(typeid(intype))) \
            return new Angle<intype, outtype>(dtype.dimension());
    #define ifTypeDeclareFactory(type) \
        ifTypeDeclareFactory_(std::complex<type>, type)
    ifTypeDeclareFactory(double);
    ifTypeDeclareFactory(float);
    ifTypeDeclareFactory(int64_t);
    ifTypeDeclareFactory(int32_t);
    ifTypeDeclareFactory(int16_t);
    ifTypeDeclareFactory(int8_t);
    throw Pothos::InvalidArgumentException("angleFactory("+dtype.toString()+")", "unsupported type");
}

static Pothos::BlockRegistry registerAngle(
    "/comms/angle", &angleFactory);
