#!/bin/bash

#draw plots from specified histoFiles then send them on the website

histDirA=${1}
histDirB=${2}
legendA=${3}
legendB=${4}
Outdir=${5}

cd ~/CMSSW_10_6_0/src/JetMET_studies/analysis
./my_DrawStuff $histDirA fusedTree $histDirB fusedTree $legendA $legendB ~/CMSSW_10_6_0/src/JetMET_studies/analysis/plots/$Outdir

./scripts/send_to_website.sh plots/$Outdir $Outdir

cd -
