import sys 
import os
import glob
import ROOT
from argparse import ArgumentParser

'''
   This script aims at simplifying the syntax of the launch command to produce and compare the jet and met distributions of two files

   The files can be of different kinds: 
      - not local: this means that they will be fetched using the dasgoclient
        -> in this case, one has to specify if the file is data (MINIAOD) or MC (MINIAODSIM)

      - local: one of our private file produced with the -c QCD options in
        https://github.com/pfclustering/RecoSimStudies/blob/am-branch/Dumpers/test/ECALproductionHelper/prodHelper.py


   -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

   The general syntax of the launch command is
 
   source scripts/draw_from_root.sh [isLocal1/isNotLocal1] [Data|MC/prodLabel1] [campaign1/-] [release1/-] [tag1/-] [nEvents1] [run1] [legend1] [isLocal2/isNotLocal2] [Data|MC/prodLabel2] [campaign2/-] [release2/-] [tag2/-] [nEvents2] [run2] [legend2] [pTcut] [repName] [doAnalyserOnly] [doPlotterOnly] 

   -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

   How to used this script:
   - first insert the information in the User's decision board
   
   - then do 'python JetMETValidator.py' if you wish to print and execute the command

     or 

     'python JetMETValidator.py --printOnly' if you don't want to execute the command 

'''

#"----------------User's decision board-----------------"

### information of file 1 ###

# a) is the file privately produced [yes/no]? 
isLocal1 = 'no'

# -> if yes: 
prodLabel1 = 'QCD_wPU_noiseCond2023_pfrhRef_seedRef_thrXtalEBXtalEE_shs1.0_maxd10.0_y2023_J4_v3_t0_n50000'

# -> if no:
# is it 'Data' or 'MC' ? 
fileType1 = 'Data'
# enter file info: (campaign/release-tag)
campaign1 = 'JetHT' #'RelValQCD_FlatPt_15_3000HS_13'
release1 = 'Run2016B' #'CMSSW_10_6_8'
tag1 = 'ForValUL2016-v1' # 'FlatPU0to70_106X_mcRun2_asymptotic_preVFP_v3_UL16_CP5_preVFP-v1'

# b) file legend:
legend1 = 'wPU'

# c) specific run [insert runId or '-'] ?
whichRun1 = '-' #'278808' 


### information of file 2 ###

# a) is the file privately produced [yes/no]? 
isLocal2 = 'yes'

# -> if yes: 
prodLabel2 = 'QCD_noPU_noiseCond2023_pfrh3.0-4.0_seed3.0-4.0_thrRingEBXtalEE_shs1.0_maxd10.0_y2023_J4_v1_t3-4_n50000'

# -> if no:
# is it 'Data' or 'MC' ? 
fileType2 = 'MC'
# enter file info: (campaign/release-tag)
campaign2 = 'RelValQCD_FlatPt_15_3000HS_13' #'JetHT'
release2 = 'CMSSW_10_6_8' #'Run2016B'
tag2 = 'FlatPU0to70_106X_mcRun2_asymptotic_v9_UL16_CP5_postVFP-v2' #'ForValUL2016-v1'

# b) file legend:
legend2 = 'noPU'

# c) specific run [insert runId or '-'] ?
whichRun2 = '-'


### other information ###
# number of events per file:
nEvents = '43097'

# apply cut on pT (in GeV)
ptCutOff = '30'

# where to save the plots? 
repName = 'J4_v3_t0_vs_t3-4'

# finally, in case you don't want to run the full machinery (analyser + drawer), please choose (only one set to True at a time):
doAnalyserOnly = False
doPlotterOnly = False

#'------------------------------------------------------'






# first, get the parsed argument, if exisiting
parser = ArgumentParser(description='Script to produce the command to run the JetMET validation tools', add_help=True)
parser.add_argument('--printOnly', dest='printOnly', help='add this option in case you want the script to only print the command without executing it', action='store_true', default=False)
option =  parser.parse_args()


# some functions
def localLabel(input):
   if input == 'yes':
      locLabel = 'isLocal'
   elif input == 'no':
      locLabel = 'isNotLocal'
   else:
      print ' ERROR: please enter [yes/no] for the isLocal variable'
      print ' --> Abortion'
      exit(11)
   return locLabel

def typeLabel(input):
   if input=='Data':
      typeLabel = 'Data'
   elif input=='MC':
      typeLabel = 'MC'
   else:
      print ' ERROR: please enter [Data/MC] for the fileType  variable'
      print ' --> Abortion'
      exit(11)
   return typeLabel




# definition of the labels 
label_a = localLabel(isLocal1)
if label_a == 'isLocal':
   label_b = prodLabel1
   label_c = '-'
   label_d = '-'
   label_e = '-'
else:
   label_b = typeLabel(fileType1)
   label_c = campaign1
   label_d = release1
   label_e = tag1
label_f = nEvents #replace by function in case doMerge
label_g = whichRun1
label_h = legend1 

label_i = localLabel(isLocal2)
if label_i == 'isLocal':
   label_j = prodLabel2
   label_k = '-'
   label_l = '-'
   label_m = '-'
else:
   label_j = typeLabel(fileType2)
   label_k = campaign2
   label_l = release2
   label_m = tag2
label_n = nEvents
label_o = whichRun2
label_p = legend2

label_q = ptCutOff
label_r = repName 

label_s = doAnalyserOnly
label_t = doPlotterOnly

# produce the command: 
command = 'source scripts/draw_from_root.sh {a} {b} {c} {d} {e} {f} {g} {h} {i} {j} {k} {l} {m} {n} {o} {p} {q} {r} {s} {t}'.format(a=label_a, b=label_b, c=label_c, d=label_d, e=label_e, f=label_f, g=label_g, h=label_h, i=label_i, j=label_j, k=label_k, l=label_l, m=label_m, n=label_n, o=label_o, p=label_p, q=label_q, r=label_r, s=label_s, t=label_t)

print '\n'
print "Running command: \n"
print command
print '\n'

if not option.printOnly:
   os.system(command)



