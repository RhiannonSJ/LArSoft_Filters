#!/bin/bash

INPUT_FILE=/sbnd/data/users/rsjones/cczeropi_outputs/cczeropi_events_rerun_reco_1.root
N_EVENTS=50

lar -c run_hitFilter.fcl -s ${INPUT_FILE} -n ${N_EVENTS}
