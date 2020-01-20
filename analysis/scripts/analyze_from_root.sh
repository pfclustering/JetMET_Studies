#!/bin/bash

echo "entering analyze_from_root script"

echo "check the proxy"

runDir="/afs/cern.ch/user/a/anlyon/CMSSW_10_6_0/src/JetMET_Studies/analysis"

#proxy_test=`voms-proxy-info |& cut -d ':' -f 1`

#if [ "$proxy_test" == "Proxy not found" ]
#then
#    echo 'Proxy not found, calling setAAA.sh to set-up a new one'
#    echo
    #source setAAA.sh
#fi

echo "proxy check done"

#making a DAS query to find dataset matching entry parameters, you can find more information at https://cmsweb.cern.ch/das/
#dataset=/${1}/${2}-${3}/NANOAODSIM
dataset=/${1}/${2}-${3}/MINIAODSIM
echo $dataset
dasgoclient --query="file dataset=$dataset | grep file.nevents | grep file.name" > liste
less liste
number_of_line=`wc -l liste | cut -d ' ' -f 1`
echo "number of line: " $number_of_line

echo "DAS query done"

#Looping on every dataset found until we got at least 50 000 events if possible
#This loop parses the query output and calls my_analyzer.cpp for every dataset
events=0
i=0
maxloop=10
maxevents=50000

echo "loop on the events"

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
    #targetDir=/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/${1}/${2}/${3}

    #echo "Will create the target dir"
    #mkdir /eos/user/a/anlyon/www/JetMET_validation/${1}/${2}/${3} #RelValQCD_FlatPt_15_3000HS_13
    targetDir=/eos/user/a/anlyon/www/JetMET_validation/${1}/${2}/${3}
    
    cutOff=0

    rm line.tmp #comment to keep line.tmp for debugging purpose
    
    
    declare datasetName$i=$datasetName
   
    echo "will source my_analyzer"
    #cd ..
    #source my_analyzer $prodName $datasetName $targetDir
    echo "check:"
    echo "productionName: " $prodName
    echo "datasetName: " $datasetName
    echo "targetDir: " $targetDir
    #cd ..
    ./new_analyzer $prodName $datasetName $targetDir $cutOff
    #cd $runDir
done

rm liste

echo "Will create the target dir"
mkdir -p /eos/user/a/anlyon/www/JetMET_validation/${1}/${2}/${3}/histoFiles

#Here we merge all the .root files produced by my_analyzer
un=1
if [ $number_of_line -gt $un ]
then
    cd $targetDir/histoFiles
    hadd -f fusedTree.root $datasetName1 $datasetName2 $datasetName3 $datasetName4 $datasetName5
else
    cd $targetDir/histoFiles
    cp $datasetName fusedTree.root

    cd -
fi

echo $targetDir

cd $runDir

#echo "Exiting the analyze_from_root script"
