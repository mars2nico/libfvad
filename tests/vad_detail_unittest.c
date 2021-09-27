/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2016 Daniel Pirch.
 *  copyright (c) 2021 mars2nico.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "vad_unittest.h"
#include "../include/fvad.h"
#include "../include/fvad_detail.h"
#include "../src/vad/vad_core.h"

#include <stdio.h>


// Returns true if the rate and frame length combination is valid.
bool ValidRatesAndFrameLengths(int rate, size_t frame_length) {

  if (rate == 8000) {
    if (frame_length == 80 || frame_length == 160 || frame_length == 240) {
      return true;
    }
    return false;
  } else if (rate == 16000) {
    if (frame_length == 160 || frame_length == 320 || frame_length == 480) {
      return true;
    }
    return false;
  } else if (rate == 32000) {
    if (frame_length == 320 || frame_length == 640 || frame_length == 960) {
      return true;
    }
    return false;
  } else if (rate == 48000) {
    if (frame_length == 480 || frame_length == 960 || frame_length == 1440) {
      return true;
    }
    return false;
  }

  return false;
}


#ifdef TEST_VAD_DETAIL_API
void test_main() {
  // This API test runs through the additional APIs for all possible valid and invalid
  // combinations.

  Fvad* handle = fvad_new();
  FvadDetail dst;
  FvadConstNumber constant_num;
  int numChannel;
  int channel;
  int16_t zeros[kMaxFrameLength] = { 0 };

  // Construct a speech signal that will trigger the VAD in all modes. It is
  // known that (i * i) will wrap around, but that doesn't matter in this case.
  int16_t speech[kMaxFrameLength];
  for (size_t i = 0; i < kMaxFrameLength; i++) {
    speech[i] = i * i;
  }

  // fvad_detail_get_channels() tests.
  EXPECT_TRUE(numChannel = fvad_detail_get_channels(handle));

  // fvad_detail_get() invalid channels tests. Tries smallest value
  // minus one and largest value plus one.
  ASSERT_EQ(NULL, fvad_detail_get(handle, -1, &dst));
  ASSERT_EQ(NULL, fvad_detail_get(handle, numChannel, &dst));

  // fvad_detail_get_const() invalid channels tests. Tries smallest value
  // minus one and largest value plus one.
  ASSERT_EQ(NULL, fvad_detail_get_const(handle, -1, &constant_num));
  ASSERT_EQ(NULL, fvad_detail_get_const(handle, numChannel, &constant_num));

  // fvad_detail_get() and fvad_detail_get_const() tests
  // All zeros as input should work
  fvad_set_sample_rate(handle, kRates[0]);
  for (channel = 0; channel < numChannel; ++channel) {
    EXPECT_EQ(&constant_num, fvad_detail_get_const(handle, channel, &constant_num));
    EXPECT_TRUE(constant_num.kSpectrumWeight);
  }
  fvad_process(handle, zeros, kFrameLengths[0]);
  for (channel = 0; channel < numChannel; ++channel) {
    EXPECT_EQ(&dst, fvad_detail_get(handle, channel, &dst));
    EXPECT_EQ(0, dst.h0_test);
    EXPECT_EQ(0, dst.h1_test);
  }

  for (size_t k = 0; k < kModesSize; k++) {
    fvad_set_mode(handle, kModes[k]);
    // Loop through sampling rate and frame length combinations
    for (size_t i = 0; i < kRatesSize; i++) {
      for (size_t j = 0; j < kFrameLengthsSize; j++) {
        if (ValidRatesAndFrameLengths(kRates[i], kFrameLengths[j])) {
          fvad_set_sample_rate(handle, kRates[i]);
          fvad_process(handle, speech, kFrameLengths[j]);
          for (channel = 0; channel < numChannel; ++channel) {
            EXPECT_EQ(&dst, fvad_detail_get(handle, channel, &dst));
          }
        }
      }
    }
  }

  // Iterates same tests
  fvad_set_sample_rate(handle, kRates[0]);
  // supress total_power
  for (int i = 0; i < 40; ++i) { // TODO(mars2nico) fix hard coding
    fvad_process(handle, zeros, kFrameLengths[0]);
  }
  for (channel = 0; channel < numChannel; ++channel) {
    EXPECT_EQ(&dst, fvad_detail_get(handle, channel, &dst));
    EXPECT_EQ(0, dst.h0_test);
    EXPECT_EQ(0, dst.h1_test);
  }

  for (size_t k = 0; k < kModesSize; k++) {
    fvad_set_mode(handle, kModes[k]);

    for (size_t i = 0; i < kRatesSize; i++) {
      for (size_t j = 0; j < kFrameLengthsSize; j++) {
        if (ValidRatesAndFrameLengths(kRates[i], kFrameLengths[j])) {
          fvad_set_sample_rate(handle, kRates[i]);
          fvad_process(handle, speech, kFrameLengths[j]);
          for (channel = 0; channel < numChannel; ++channel) {
            EXPECT_EQ(&dst, fvad_detail_get(handle, channel, &dst));
          }
        }
      }
    }
  }

  fvad_free(handle);
}
#endif // TEST_VAD_DETAIL_API
