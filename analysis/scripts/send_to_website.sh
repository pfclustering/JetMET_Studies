#!/bin/bash

#takes a plot directory and send it on the website under a specified name

mkdir /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/${2}
cp ${1}/* /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/${2}
cp /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/index.php /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/${2}
