#!/bin/bash

#if [ $#!=9 ]
#then
#    echo "This script needs 9 parameters : ./draw_from_root campaign1 release1 globalTag1 legend1 campaign2 release2 globalTag2 legend2 plotsName"
#   exit 0
#fi

#calling analyze_from_root.sh for both dataset and storing in listeA and listeB the location of the produced .root files 
./analyze_from_root.sh ${1} ${2} ${3} > listeA

tail -n 1 listeA>listeA.tmp
mv listeA.tmp listeA

./analyze_from_root.sh ${5} ${6} ${7} > listeB

tail -n 1 listeB>listeB.tmp
mv listeB.tmp listeB

prodName1=`less listeA`
prodName2=`less listeB`

datasetName1=fusedTree
datasetName2=fusedTree
legend1=${4}
legend2=${8}
plotsName=${9}


rm listeA
rm listeB

./my_DrawStuff $prodName1 $datasetName1 $prodName2 $datasetName2 $legend1 $legend2 $plotsName 

cd /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/$plotsName

echo 'plots created with :' > specs
echo $legend1'='$prodName1 >> specs
echo $legend2'='$prodName2 >> specs

cp ../index.php .

cd -
