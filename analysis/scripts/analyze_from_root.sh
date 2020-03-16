#!/bin/bash

#make sure that the CMSSW release is the correct one
runDir="/t3home/anlyon/CMSSW_10_6_8/src/JetMET_Studies/analysis"

# check is the file is local or not
if [ "${1}" == "isLocal" ] ; then
   isLocal=true
else
   isLocal=false
fi

# different format file between MC and data
if [ "$isLocal" = false ] ; then
   if [ "${2}" == "Data" ] ; then
      dataset=/${3}/${4}-${5}/MINIAOD
   else
      dataset=/${3}/${4}-${5}/MINIAODSIM
   fi
else
   dataset=${2}
fi

echo $dataset

# define the output directorie
if [ "$isLocal" = false ] ; then
   targetDir=/scratch/anlyon/JetMET_validation/${3}/${4}/${5}
else
   targetDir=/scratch/anlyon/JetMET_validation/${2}
fi

mkdir -p $targetDir/histoFiles
# clean the targetDir
rm -r $targetDir/histoFiles/*

maxevents=${6}
cutOff=${8}  #cut on jet pt, in GeV

# ---- if sample is not local ---- #
if [ "$isLocal" = false ] ; then
   #making a DAS query to find dataset matching entry parameters, you can find more information at https://cmsweb.cern.ch/das/
   if [ ${7} == '-' ] ; then
      dasgoclient --query="file dataset=$dataset | grep file.nevents | grep file.name" > liste
   else
      dasgoclient --query="file dataset=$dataset run=${7} | grep file.nevents | grep file.name" > liste
   fi

   less liste
   number_of_line=`wc -l liste | cut -d ' ' -f 1`
   echo "number of line: " $number_of_line
   echo "DAS query done"


   #Looping on every dataset found until we got at least maxevents if possible
   #This loop parses the query output and calls new_analyzer.cpp for every dataset

   events=0
   nEvtDone=0

   i=0
   while [ $events -lt $maxevents ] && [ $i -lt $number_of_line ]
   do
       echo "i: " $i
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

       # directory where the nanoAOD files are stored (when privately produced)
       nanoFileDir=/pnfs/psi.ch/cms/trivcat/store/user/anlyon/JetMET_production/${3}_${4}-${5}/$datasetName

       rm line.tmp #comment to keep line.tmp for debugging purpose
       
       echo "will source my_analyzer"
       echo "productionName: " $prodName
       echo "datasetName: " $datasetName
       echo "targetDir: " $targetDir
       echo "nanoFileDir: " $nanoFileDir 

       # number of events per file:
       echo "$(( $maxevents-$events))"
       if [ $(($maxevents-$events)) -gt 0 ] ; then
         nEvt=$nevents
         nEvtDone=$(($nEvtDone+$nevents))
         echo "cat 1"
       else
         nEvt=$(($maxevents-$nEvtDone))
         echo "cat2"
       fi
       
       ./new_analyzer $prodName $datasetName $nanoFileDir $targetDir $nEvt $cutOff
       
       # adding the file to fusedTree.root 
       cd $targetDir/histoFiles
     
       if [ $i == 1 ] ; then
         cp $datasetName fusedTree.root
       elif [ $i -gt 1 ] ; then
         hadd -a fusedTree.root $datasetName
       fi
      
       cd $runDir
   done

   rm liste
fi

# ---- if sample is local ---- #
if [ "$isLocal" = true ] ; then

   nanoFileDir=/pnfs/psi.ch/cms/trivcat/store/user/anlyon/JetMET_production/${2}
   datasetName=$dataset
   prodName="private"

   echo "will source my_analyzer"
   echo "datasetName: " $datasetName
   echo "targetDir: " $targetDir
   echo "nanoFileDir: " $nanoFileDir 
   echo "number of events: " $maxevents

   ./new_analyzer $prodName $datasetName $nanoFileDir $targetDir $maxevents $cutOff
   cp $targetDir/histoFiles/$datasetName".root" $targetDir/histoFiles/fusedTree.root
fi
echo $targetDir

cd $runDir

