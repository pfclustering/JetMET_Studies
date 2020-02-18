#!/bin/bash

# compile the files
make all

# processing the first file
echo "Will analyze the first file"

if [ "${1}" == "data" ] ; then
   echo "data: "${2}_${3}-${4}
   source scripts/analyze_from_root.sh ${2} ${3} ${4} Data > listeA
elif [ "${1}" == "MC" ] ; then
   echo "MC: "${2}_${3}-${4}
   source scripts/analyze_from_root.sh ${2} ${3} ${4} MC > listeA
else
   echo "Couldn't recognize if the first sample is Data or MC"
   echo "Please make sure that the first parsed string is either 'data' or 'MC'" 
   echo "-->Aborting"
   kill %%
fi

rm listeA

echo "Done with the analyzing the first file"

# processing the second file
echo "Will analyze the second file"

if [ "${6}" == "data" ] ; then
   echo "data: "${7}_${8}-${9}
   source scripts/analyze_from_root.sh ${7} ${8} ${9} Data > listeB
elif [ "${6}" == "MC" ] ; then
   echo "MC: "${7}_${8}-${9}
   source scripts/analyze_from_root.sh ${7} ${8} ${9} MC > listeB
else
   echo "Couldn't recognize if the first sample is Data or MC"
   echo "Please make sure that the first parsed string is either 'data' or 'MC'" 
   echo "-->Aborting"
   kill %%
fi

rm listeB

echo "Done with the analyzing the second file"


# setting drawing parameters

my_prodName1="/scratch/anlyon/JetMET_validation/"${2}/${3}/${4}
my_prodName2="/scratch/anlyon/JetMET_validation/"${7}/${8}/${9}
echo "prodName1: " $my_prodName1
echo "prodName2: " $my_prodName2

datasetName1=fusedTree
datasetName2=fusedTree
legend1=${5}
legend2=${10}
plotsName=${11}

echo "Will run the drawing script"
./my_DrawStuff $my_prodName1 $datasetName1 $my_prodName2 $datasetName2 $legend1 $legend2 $plotsName 
echo "Done with the drawing script"

echo 'plots created with :' > $plotsName/specs
echo $legend1'='${2}_${3}-${4} >> $plotsName/specs
echo $legend2'='${7}_${8}-${9} >> $plotsName/specs

cp index.php $plotsName

cd -
