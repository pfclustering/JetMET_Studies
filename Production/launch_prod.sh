#!/bin/bash

#sbatch -p wn --account=t3 -o logs/prod.log -e logs/prod.log --job-name=nanoAOD --ntasks=10 launch_prod.sh


doMC=false
doData=true

if [ "$doData" = true ] ; then
   campaign="JetHT"
   release="Run2016F"
   tag="ForValUL2016-v1"
fi

if [ "$doMC" = true ] ; then
   campaign="RelValQCD_FlatPt_15_3000HS_13"
   release="CMSSW_10_6_8"
   #tag="FlatPU0to70_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5_preVFP-v1"
   tag="FlatPU0to70_106X_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v2"
fi


nEvents=10
if [ "$nEvents" == -1 ] ; then
   echo "Going to process all the events per file"
else
   echo "Going to process " $Events " events per file"
fi


mkdir logs

# setting the global tag
if [ "$doData" = true ] ; then
   globalTag="106X_dataRun2_v25"
fi

if [ "$tag" == "FlatPU0to70_106X_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v2" ] ; then
   globalTag="106X_mcRun2_asymptotic_v9"
fi

if [ "$tag" == "FlatPU0to70_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5_preVFP-v1" ] ; then
   globalTag="106X_mcRun2_asymptotic_preVFP_v3"
fi


echo "globalTag: " $globalTag


directoryName=$campaign"_"$release"-"$tag
workDir="/scratch/anlyon/"$directoryName
runDir="/t3home/anlyon/CMSSW_10_6_0/src/JetMET_Studies/Production"
saveDirRoot="/pnfs/psi.ch/cms/trivcat/store/user/anlyon/JetMETtest"

#echo $workDir

mkdir -p $workDir
xrdfs t3dcachedb03.psi.ch mkdir $saveDirRoot

saveDir=$saveDirRoot/$campaign"_"$release"-"$tag

xrdfs t3dcachedb03.psi.ch mkdir $saveDir


if [ "$doData" = true ] ; then 
   dataset=/$campaign/$release-$tag/MINIAOD
fi

if [ "$doMC" = true ] ; then
   dataset=/$campaign/$release-$tag/MINIAODSIM
fi
echo $dataset
dasgoclient --query="file dataset=$dataset run=278801 | grep file.nevents | grep file.name" > liste
#less liste
number_of_line=`wc -l liste | cut -d ' ' -f 1`


events=0
i=0
maxloop=10
maxevents=1000000


while [ $events -lt $maxevents ] && [ $i -lt $number_of_line ]
do
    let "i = $i +1"
    head -n $i liste | tail -1 > line.tmp
        
    test=`cut -d '/' -f 2 line.tmp`
    if [ $test != "store" ]
    then
	echo "query failed, here is its result:"
	less line.tmp
	rm l*
	exit 0
    fi

    nevents=`cut -d " " -f 1 line.tmp`
    let "events=$events+$nevents"

    prodName=`cut -d " " -f 4 line.tmp`

    datasetName=`cut -d '/' -f 9 line.tmp | less`

    outputDir=$prodName


    rm line.tmp 
    
    declare datasetName$i=$datasetName
   
    echo "productionName: " $prodName
    echo "datasetName: " $datasetName
   
    mkdir -p $workDir/$outputDir
    cp cmsDriver/MiniAODSIMtoNanoAODSIM.py $workDir/$outputDir
    cp cmsDriver/MiniAODtoNanoAOD.py $workDir/$outputDir
    cd $workDir/$outputDir
   
    if [ "$doData" = true ] ; then
       echo "processing data"
       echo "productionName: " $prodName
       echo "datasetName: " $datasetName
       echo "globalTag: " $globalTag
       cmsRun MiniAODtoNanoAOD.py nevents=$nEvents productionName=$prodName dataset=$datasetName globalTag=$globalTag 
    fi

    if [ "$doMC" = true ] ; then
      cmsRun MiniAODSIMtoNanoAODSIM.py nevents=$nEvents productionName=$prodName dataset=$datasetName globalTag=$globalTag
    fi
    #echo "I run driver" >> file.txt

    saveSEDir=$saveDir/$datasetName
    xrdfs t3dcachedb03.psi.ch mkdir $saveSEDir

    xrdcp -f file.root root://t3dcachedb.psi.ch:1094/$saveSEDir
    
    # empty the scratch
    cd $workDir/$outputDir
    rm *
    cd $runDir
done

rm liste



