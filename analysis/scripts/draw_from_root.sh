#!/bin/bash

# compile the files
make all

# PART 1: Analyser #

if [ ${20} != "True" ] ; then # we analyse the files only if doPlotterOnly=False
   # processing the first file
   echo "Will analyze the first file"

   # centrally produced files:
   if [ "${1}" != "isLocal" ] ; then
      if [ "${2}" == "Data" ] ; then
         echo "data: "${3}_${4}-${5}
      elif [ "${2}" == "MC" ] ; then
         echo "MC: "${3}_${4}-${5}
      fi
   else # samples privately produced:
      echo "sample: "${2}
   fi
   source scripts/analyze_from_root.sh ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${17}> listeA
   rm listeA

   echo "Done with the analyzing the first file"

   # processing the second file
   echo "Will analyze the second file"

   # centrally produced files:
   if [ "${9}" != "isLocal" ] ; then
      if [ "${10}" == "Data" ] ; then
         echo "data: "${11}_${12}-${13}
         elif [ "${10}" == "MC" ] ; then
            echo "MC: "${11}_${12}-${13}
         fi
      else # samples privately produced:
         echo "sample: "${10}
   fi
   source scripts/analyze_from_root.sh ${9} ${10} ${11} ${12} ${13} ${14} ${15} ${17} > listeB
   rm listeB

   echo "Done with the analyzing the second file"
fi

# PART2: DRAW #

if [ ${19} != "True" ] ; then # we run the drawer only if doAnalyserOnly is False
   # setting drawing parameters

   if [ "${1}" != "isLocal" ] ; then
      my_prodName1="/scratch/anlyon/JetMET_validation/"${3}/${4}/${5}
   else
      my_prodName1="/scratch/anlyon/JetMET_validation/"${2}
   fi
   #if [ "$startForSecondFile" = 6 ] ; then
   if [ ${9} != "isLocal" ] ; then
      my_prodName2="/scratch/anlyon/JetMET_validation/"${11}/${12}/${13}
   else
      my_prodName2="/scratch/anlyon/JetMET_validation/"${10}
   fi


   echo "prodName1: " $my_prodName1
   echo "prodName2: " $my_prodName2

   datasetName1=fusedTree
   datasetName2=fusedTree

   legend1=${8}
   legend2=${16}
   plotsName=${18}


   echo "Will run the drawing script"
   ./my_DrawStuff $my_prodName1 $datasetName1 $my_prodName2 $datasetName2 $legend1 $legend2 $plotsName 
   echo "Done with the drawing script"


   echo 'plots created with :' > 'plots/'$plotsName/specs
   echo $legend1'='$my_prodName1 >> 'plots/'$plotsName/specs
   echo $legend2'='$my_prodName2 >> 'plots/'$plotsName/specs

   echo ' '  >> 'plots/'$plotsName/specs

   echo 'command:' >> 'plots/'$plotsName/specs
   command='source scripts/draw_from_root.sh'${1}' '${2}' '${3}' '${4}' '${5}' '${6}' '${7}' '${8}' '${9}' '${10}' '${11}' '${12}' '${13}' '${14}' '${15}' '${16}' '${17}  
   echo $command >> 'plots/'$plotsName/specs

   cp index.php $plotsName

   cd -
fi   
