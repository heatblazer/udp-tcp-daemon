TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    base/abs.hpp \
    base/asin_acos.hpp \
    base/atan.hpp \
    base/basic_expressions.hpp \
    base/clamp.hpp \
    base/compiletime.hpp \
    base/complex.hpp \
    base/constants.hpp \
    base/cpuid.hpp \
    base/cpuid_auto.hpp \
    base/digitreverse.hpp \
    base/expression.hpp \
    base/function.hpp \
    base/gamma.hpp \
    base/generators.hpp \
    base/hyperbolic.hpp \
    base/intrinsics.h \
    base/kfr.h \
    base/log_exp.hpp \
    base/logical.hpp \
    base/memory.hpp \
    base/min_max.hpp \
    base/modzerobessel.hpp \
    base/operators.hpp \
    base/platform.hpp \
    base/pointer.hpp \
    base/random.hpp \
    base/read_write.hpp \
    base/reduce.hpp \
    base/round.hpp \
    base/saturation.hpp \
    base/select.hpp \
    base/shuffle.hpp \
    base/simd.hpp \
    base/sin_cos.hpp \
    base/small_buffer.hpp \
    base/sort.hpp \
    base/specializations.i \
    base/sqrt.hpp \
    base/tan.hpp \
    base/types.hpp \
    base/univector.hpp \
    base/vec.hpp \
    cometa/array.hpp \
    cometa/cstring.hpp \
    cometa/ctti.hpp \
    cometa/function.hpp \
    cometa/named_arg.hpp \
    cometa/range.hpp \
    cometa/result.hpp \
    cometa/string.hpp \
    cometa/tuple.hpp \
    data/bitrev.hpp \
    data/sincos.hpp \
    dft/bitrev.hpp \
    dft/cache.hpp \
    dft/conv.hpp \
    dft/fft.hpp \
    dft/ft.hpp \
    dft/reference_dft.hpp \
    dsp/biquad.hpp \
    dsp/biquad_design.hpp \
    dsp/dcremove.hpp \
    dsp/delay.hpp \
    dsp/fir.hpp \
    dsp/fir_design.hpp \
    dsp/fracdelay.hpp \
    dsp/goertzel.hpp \
    dsp/impulse.hpp \
    dsp/interpolation.hpp \
    dsp/mixdown.hpp \
    dsp/oscillators.hpp \
    dsp/resample.hpp \
    dsp/sample_rate_conversion.hpp \
    dsp/speaker.hpp \
    dsp/units.hpp \
    dsp/waveshaper.hpp \
    dsp/weighting.hpp \
    dsp/window.hpp \
    io/audiofile.hpp \
    io/file.hpp \
    io/python_plot.hpp \
    io/tostring.hpp \
    all.hpp \
    base.hpp \
    cident.h \
    cometa.hpp \
    dft.hpp \
    dsp.hpp \
    io.hpp \
    math.hpp \
    version.hpp

SOURCES += \
    main.cpp
