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

#ifndef FVAD_DETAIL_H_
#define FVAD_DETAIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "fvad.h"

typedef struct FvadConstNumber {
    int16_t kSpectrumWeight;
} FvadConstNumber;

typedef struct FvadDetail {
    int32_t h0_test; // represented as fixed point number : headding 5 bits is part of integer and trailing 27 bits is decimal points.
    int32_t h1_test; // represented as fixed point number
} FvadDetail;

FvadDetail *fvad_detail_get(Fvad* inst, int channel, FvadDetail *dst);
int fvad_detail_get_channels(Fvad* inst);
FvadConstNumber *fvad_detail_get_const(Fvad* inst, int channel, FvadConstNumber *dst);

#ifdef __cplusplus
}
#endif

#endif // FVAD_DETAIL_H_
