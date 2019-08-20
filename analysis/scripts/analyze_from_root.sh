#!/bin/bash

proxy_test=`voms-proxy-info |& cut -d ':' -f 1`

if [ "$proxy_test" == "Proxy not found" ]
then
    echo 'Proxy not found, calling setAAA.sh to set-up a new one'
    echo
    source setAAA.sh
fi

#making a DAS query to find dataset matching entry parameters, you can find more information at https://cmsweb.cern.ch/das/
dataset=/${1}/${2}-${3}/NANOAODSIM
echo $dataset
dasgoclient --query="file dataset=$dataset | grep file.nevents | grep file.name" > liste
less liste
number_of_line=`wc -l liste | cut -d ' ' -f 1`

#Looping on every dataset found until we got at least 50 000 events if possible
#This loop parses the query output and calls my_analyzer.cpp for every dataset
events=0
i=0
maxloop=10
maxevents=50000

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
    targetDir=/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/${1}/${2}/${3}
    
    rm line.tmp #comment to keep line.tmp for debugging purpose
    
    
    declare datasetName$i=$datasetName
 
    ./my_analyzer $prodName $datasetName $targetDir
done

rm liste

#Here we merge all the .root files produced by my_analyzer
un=1
if [ $number_of_line -gt $un ]
then
    cd $targetDir/histoFiles
    hadd -f fusedTree.root $datasetName1 $datasetName2 $datasetName3 $datasetName4 $datasetName5
else
    cd $targetDir/histoFiles
    cp $datasetName1 fusedTree.root

    cd -
fi

echo $targetDir
