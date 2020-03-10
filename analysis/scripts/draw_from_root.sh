#!/bin/bash

# compile the files
make all

# processing the first file
echo "Will analyze the first file"

# centrally produced files:
if [ "${1}" != "isLocal" ] ; then
   startForSecondFile=6
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
else # samples privately produced:
   startForSecondFile=4
   echo "sample: "${2}
   source scripts/analyze_from_root.sh ${1} ${2} > listeA
fi
rm listeA

echo "Done with the analyzing the first file"

# processing the second file
echo "Will analyze the second file"

if [ "$startForSecondFile" = 6 ] ; then
   # centrally produced files:
   if [ "${6}" != "isLocal" ] ; then
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
   else # samples privately produced:
      echo "sample: "${7}
      source scripts/analyze_from_root.sh ${6} ${7} > listeB
   fi
elif [ "$startForSecondFile" = 4 ] ; then
   # centrally produced files:
   if [ "${4}" != "isLocal" ] ; then
      if [ "${4}" == "data" ] ; then
         echo "data: "${5}_${6}-${7}
         source scripts/analyze_from_root.sh ${5} ${6} ${7} Data > listeB
      elif [ "${4}" == "MC" ] ; then
         echo "MC: "${5}_${6}-${7}
         source scripts/analyze_from_root.sh ${5} ${6} ${7} MC > listeB
      else
         echo "Couldn't recognize if the first sample is Data or MC"
         echo "Please make sure that the first parsed string is either 'data' or 'MC'" 
         echo "-->Aborting"
         kill %%
      fi
   else # samples privately produced:
      echo "sample: "${5}
      source scripts/analyze_from_root.sh ${4} ${5} > listeB
   fi
fi
rm listeB

echo "Done with the analyzing the second file"


# setting drawing parameters

if [ "${1}" != "isLocal" ] ; then
   my_prodName1="/scratch/anlyon/JetMET_validation/"${2}/${3}/${4}
else
   my_prodName1="/scratch/anlyon/JetMET_validation/"${2}
fi
if [ "$startForSecondFile" = 6 ] ; then
   if [ ${6} != "isLocal" ] ; then
      my_prodName2="/scratch/anlyon/JetMET_validation/"${7}/${8}/${9}
   else
      my_prodName2="/scratch/anlyon/JetMET_validation/"${7}
   fi
elif [ "$startForSecondFile" = 4 ] ; then
   if [ ${4} != "isLocal" ] ; then
      my_prodName2="/scratch/anlyon/JetMET_validation/"${5}/${6}/${7}
   else
      my_prodName2="/scratch/anlyon/JetMET_validation/"${5}
   fi
fi


echo "prodName1: " $my_prodName1
echo "prodName2: " $my_prodName2

datasetName1=fusedTree
datasetName2=fusedTree

if [ "${1}" != "isLocal" ] ; then
   legend1=${5}
else
   legend1=${3}
fi
if [ "$startForSecondFile" = 6 ] ; then
   if [ ${6} != "isLocal" ] ; then
      legend2=${10}
      plotsName=${11}
   else
      legend2=${8}
      plotsName=${9}
   fi
elif [ "$startForSecondFile" = 4 ] ; then
   if [ ${4} != "isLocal" ] ; then
      legend2=${8}
      plotsName=${9}
   else
      legend2=${6}
      plotsName=${7}
   fi
fi


echo "Will run the drawing script"
./my_DrawStuff $my_prodName1 $datasetName1 $my_prodName2 $datasetName2 $legend1 $legend2 $plotsName 
echo "Done with the drawing script"

echo 'plots created with :' > $plotsName/specs
echo $legend1'='$my_prodName1 >> $plotsName/specs
echo $legend2'='$my_prodName2 >> $plotsName/specs

cp index.php $plotsName

cd -
