#!/bin/bash

#if [ $#!=9 ]
#then
#    echo "This script needs 9 parameters : ./draw_from_root campaign1 release1 globalTag1 legend1 campaign2 release2 globalTag2 legend2 plotsName"
#   exit 0
#fi

./analyze_from_root.sh ${1} ${2} ${3} > liste1

tail -n 1 liste1>liste1.tmp
mv liste1.tmp liste1

./analyze_from_root.sh ${5} ${6} ${7} > liste22

tail -n 1 liste22>liste22.tmp
mv liste22.tmp liste22

prodName1=`less liste1`
prodName2=`less liste22`

datasetName1=fusedTree
datasetName2=fusedTree
legend1=${4}
legend2=${8}
plotsName=${9}


rm liste1
rm liste22

./my_DrawStuff $prodName1 $datasetName1 $prodName2 $datasetName2 $legend1 $legend2 $plotsName 

cd /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/$plotsName

echo 'plots created with :' > specs
echo $legend1'='$prodName1 >> specs
echo $legend2'='$prodName2 >> specs

cp ../index.php .

cd -