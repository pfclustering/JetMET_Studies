# JetMET_Studies
Scripts to send online plots useful to JetMET validation

## Installation
Clone this repository in a CMSSW release, preferably CMSSW_10_6_0.
```bash
git clone https://github.com/martinguillot/JetMET_Studies.git
cd JetMET_Studies/analysis
```
## Running the script locally
### Modifying the code
This script automatically submit the plots here: http://ecaldpg.web.cern.ch/ecaldpg/JetMET_validation/mguillot/plots/

If you want to run this locally, you will need to edit a few lines:
In analyse_from_root.sh at line 44 change the target directory so that the root files are created where you want them to be created. Let's call your directory ```testDir```. 


In my_DrawStuff.cpp at line 55
```
std::string outdir( Form("/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/%s", comparisonName.c_str()) );
```
Just delete ```/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/``` so that you plots are created in your working directory.

Finally in draw_from_root.sh at line 34
```cd /eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/$plotsName```
Just delete ```/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/``` and comment the line 40.
### Run the script
Here is an exemple to test the script.

First set up your proxy and compile the code :
```
source setAAA.sh
make Makefile
```
then run 
```
./draw_from_root.sh
