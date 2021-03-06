/*
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * Reads the .rep from -input and dumps it to -output.
 */

#include "state.h"
#include "replayer.h"

#include <torchcraft/replayer.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

using namespace cherrypi;

DEFINE_string(input, ".", "Use this starcraft replay file");
DEFINE_string(output, ".", "Dump it out here");

int main(int argc, char** argv) {
  cherrypi::init();
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  cherrypi::initLogging(argv[0], "", true);

  torchcraft::replayer::Replayer tcrep;

  Replayer replay(FLAGS_input);
  replay.setCombineFrames(3);
  replay.init();
  auto state = replay.state();

  tcrep.setMapFromState(state->tcstate());

  while (!state->gameEnded()) {
    state->tcstate()->frame->creep_map.clear();
    tcrep.push(state->tcstate()->frame);
    replay.step();
  }
  tcrep.push(state->tcstate()->frame);
  tcrep.setKeyFrame(-1);

  tcrep.save(FLAGS_output, true);

  return 0;
}
