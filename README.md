# JetMET_Studies
Scripts to send online plots useful to JetMET validation

## Installation
Clone this repository in a CMSSW release, preferably CMSSW_10_6_0.
```bash
git clone https://github.com/martinguillot/JetMET_Studies.git
cd JetMET_Studies/
```

In case you will need to interact with the SE, don't forget to setup your proxy:
```
voms-proxy-init -voms cms -rfc
```

## Production
You might need to produce NANOAOD files on top of MINIAODs produced centrally.

In this case, do
```
cd Production/
```

Then, in launch_prod.sh:
- Decide whether to run Data or MC
- Insert the campain_release-tag info so that the file can be found in DAS
- Check that the GlobalTags are the correct one
- Choose in which repository to store the files 

Once this is ready, do
```
sbatch -p wn --account=t3 -o logs/prod.log -e logs/prod.log --job-name=nanoAOD --time=5-23:59 launch_prod.sh
```

In order to merge all the nano files into a single one, do
```
python nanoMerger.py --pl <prodLabel>
```

## JetMET validator

```
cd analysis/
```

This is a set of scripts (Analyser + Plotter) that allow to produce and compare the main Jet and MET distributions.

Everything can be monitored from JetMETvalidator.py. This script produces the command line that launches the analyser+plotter for two specific files.

```
Details and instructions are given directly in JetMETValidator.py
python JetMETValidator.py
```

The main command consists in running the bash script 'scripts/draw_from_root.sh' that launches the different steps to analyse + draw the distributions. The analyser itself is launched via 'scripts/analyze_from_root.root' which gathers all the necessary information to launch the analyser 'new_analyzer.cpp'. The output of the analyser are so-called 'fusedTree.root' which contain the necessary histograms and TProfiles. The plotter 'my_DrawStuff.cpp' fetches those files and produce the plots.


Note: the message error
```
Error in <TNetXNGFile::Open>: [ERROR] Server responded with an error: [3011] No such file
```
occurs when the script checks if the files found on DAS have been processed and stored in our SE repository. It should not be interpreted as an issue from the code, which is supposed to execute normally afterwards, given that at least one file was found. 

