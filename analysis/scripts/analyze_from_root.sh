#!/bin/bash


#make sure that the CMSSW release is the correct one
runDir="/t3home/anlyon/CMSSW_10_6_8/src/JetMET_Studies/analysis"


# different format file between MC and data
if [ "${4}" == "Data" ] ; then
   dataset=/${1}/${2}-${3}/MINIAOD
else
   dataset=/${1}/${2}-${3}/MINIAODSIM
fi

echo $dataset

# define the output directorie
targetDir=/scratch/anlyon/JetMET_validation/${1}/${2}/${3}

mkdir -p $targetDir/histoFiles
# clean the targetDir
rm -r $targetDir/histoFiles

#making a DAS query to find dataset matching entry parameters, you can find more information at https://cmsweb.cern.ch/das/
dasgoclient --query="file dataset=$dataset | grep file.nevents | grep file.name" > liste
#dasgoclient --query="file dataset=$dataset run=278801 | grep file.nevents | grep file.name" > liste

less liste
number_of_line=`wc -l liste | cut -d ' ' -f 1`
echo "number of line: " $number_of_line
echo "DAS query done"



#Looping on every dataset found until we got at least 50 000 events if possible
#This loop parses the query output and calls new_analyzer.cpp for every dataset

events=0
maxevents=50000
cutOff=30 #cut on jet pt, in GeV

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
    nanoFileDir=/pnfs/psi.ch/cms/trivcat/store/user/anlyon/JetMET_production/${1}_${2}-${3}/$datasetName
    
    #cutOff=30 #cut on jet pt, in GeV

    rm line.tmp #comment to keep line.tmp for debugging purpose
    
    echo "will source my_analyzer"
    echo "productionName: " $prodName
    echo "datasetName: " $datasetName
    echo "targetDir: " $targetDir
    echo "nanoFileDir: " $nanoFileDir 
    
    ./new_analyzer $prodName $datasetName $nanoFileDir $targetDir $cutOff

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

echo $targetDir

cd $runDir

