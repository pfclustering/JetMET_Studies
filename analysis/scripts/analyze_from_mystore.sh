#!/bin/bash

#Use this script to analyse and merge root files coming from a Crab production 

inputDir=${1} #path to the root file (/store/...) without the root file own name
numberOfFiles=${2} #Number of files to merge in the input directory
filesName=${3} #General form of the files names, crab produces files like : data_1.root, data_2.root, data_3.root, ... Here filesName should be "data"
targetDir=${4} #Where you want to put your output
cutOff=${5} #Cut-off (Pt) for the analysis 

proxy_test=`voms-proxy-info |& cut -d ':' -f 1`

if [ "$proxy_test" == "Proxy not found" ]
then
    echo 'Proxy not found, calling voms-proxy-init -voms cms to set-up a new one'
    echo
    voms-proxy-init -voms cms
fi

i=1

~/CMSSW_10_6_0/src/JetMET_studies/analysis/my_analyzer "$inputDir/$filesName""_$i".root "$filesName""_$i".root $targetDir $cutOff
cd $targetDir/histoFiles
cp "$filesName""_$i".root fusedTree_1.root
cd -

while [ $i -lt $numberOfFiles ]
do
    let "i = $i +1"
    let "j = $i -1"
    ~/CMSSW_10_6_0/src/JetMET_studies/analysis/my_analyzer "$inputDir/$filesName""_$i".root "$filesName""_$i".root $targetDir $cutOff
    cd $targetDir/histoFiles
    hadd -f "fusedTree""_$i".root "fusedTree""_$j".root "$filesName""_$i".root
    rm "fusedTree""_$j".root
    cd -
done

cd $targetDir/histoFiles
mv "fusedTree""_$numberOfFiles".root fusedTree.root
rm "fusedTree""_$numberOfFiles".root
cd -

cd $targetDir
echo "Created with command ./scripts/analyze_from_mystore.sh "$inputDir" "$numberOfFiles" "$filesName" "$targetDir" "$cutOff>command.txt
cd -
