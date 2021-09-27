/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2021 mars2nico
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "../include/fvad_detail.h"

#include "vad/vad_core.h"

// TODO(mars2nico) fix redundant declaration
struct Fvad {
    VadInstT core;
    size_t rate_idx; // index in valid_rates and process_funcs arrays
};

FvadDetail *fvad_detail_get(Fvad* inst, int channel, FvadDetail *dst)
{
    assert(inst);
    assert(dst);
    if (channel < 0 || kNumChannels <= channel) return NULL;

    dst->h0_test = inst->core.h0_test[channel];
    dst->h1_test = inst->core.h1_test[channel];

    return dst;
}

int fvad_detail_get_channels(Fvad* inst)
{
    assert(inst);
    return kNumChannels;
}

FvadConstNumber *fvad_detail_get_const(Fvad* inst, int channel, FvadConstNumber *dst)
{
    assert(inst);
    assert(dst);
    if (channel < 0 || kNumChannels <= channel) return NULL;

    dst->kSpectrumWeight = inst->core.kSpectrumWeight[channel];

    return dst;
}
