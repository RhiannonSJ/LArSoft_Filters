#!/bin/bash

INPUT_FILE=/sbnd/app/users/rsjones/LArSoft_test/test_production/1000_events_genie/prodgenie_sbnd_GenieGen-20170705T202742.root
N_EVENTS=50

lar -c run_topologyFilter.fcl -s ${INPUT_FILE} -n ${N_EVENTS}
