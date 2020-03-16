import sys 
import os
import glob
import ROOT

'''
   This script aims at simplifying the syntax of the launch command to produce and compare the jet and met distributions of two files

   The files can be of different kinds: 
      - not local: this means that they will be fetched using the dasgoclient
        -> in this case, one has to specify if the file is data (MINIAOD) or MC (MINIAODSIM)

      - local: one of our private file produced with the -c QCD options in
        https://github.com/pfclustering/RecoSimStudies/blob/am-branch/Dumpers/test/ECALproductionHelper/prodHelper.py

   There is also the possibility to merge data files together (typically merge different data runs)

   -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

   The general syntax of the launch command is


   -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

   How to used this script:
   - first insert the information in the User's decision board
   
   - then do 'python JetMETValidator.py'

'''

#"----------------User's decision board-----------------"

### information of file 1 ###

# a) is the file privately produced [yes/no]? 
isLocal1 = 'yes'

# -> if yes: 
prodLabel1 = 'QCD_noPU_pfrh3.0_seed3.0_thrXtalEBXtalEE_shs1.0_maxd10.0_y2023_J1_t33_n5000'

# -> if no:
# is it 'Data' or 'MC' ? 
fileType1 = 'Data'
# enter file info: (campaign/release-tag)
campaign1 = 'JetHT'
release1 = 'Run2016H'
tag1 = 'ForValUL2016-v1'

# b) file legend:
legend1 = 'file1'

# c) specific run [insert runId or '-'] ?
whichRun1 = '-'


### information of file 2 ###
# add: doMerge

# a) is the file privately produced [yes/no]? 
isLocal2 = 'no'

# -> if yes: 
prodLabel2 = 'QCD_noPU_pfrh3.0_seed3.0_thrXtalEBXtalEE_shs1.0_maxd10.0_y2023_J1_t33_n5000'

# -> if no:
# is it 'Data' or 'MC' ? 
fileType2 = 'Data'
# enter file info: (campaign/release-tag)
campaign2 = 'JetHT'
release2 = 'Run2016H'
tag2 = 'ForValUL2016-v1'

# b) file legend:
legend2 = 'file2'

# c) specific run [insert runId or '-'] ?
whichRun2 = '-'


### other information ###
# number of events per file:
nEvents = '33000'
# note: in case file1 and file2 are merged, nEvents/2 will be taken from each file, and nEvents from file3

# apply cut on pT (in GeV)
ptCutOff = '30'

# where to save the plots? 
repName = 'test'


#'------------------------------------------------------'


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

# produce the command: 
command = 'source scripts/draw_from_root.sh {a} {b} {c} {d} {e} {f} {g} {h} {i} {j} {k} {l} {m} {n} {o} {p} {q} {r}'.format(a=label_a, b=label_b, c=label_c, d=label_d, e=label_e, f=label_f, g=label_g, h=label_h, i=label_i, j=label_j, k=label_k, l=label_l, m=label_m, n=label_n, o=label_o, p=label_p, q=label_q, r=label_r)

print '\n'
print "Running command: \n"
#print 'source scripts/draw_from_root.sh {a} {b} {c} {d} {e} {f} {g} {h} {i} {j} {k} {l} {m} {n} {o} {p} {q}'.format(a=label_a, b=label_b, c=label_c, d=label_d, e=label_e, f=label_f, g=label_g, h=label_h, i=label_i, j=label_j, k=label_k, l=label_l, m=label_m, n=label_n, o=label_o, p=label_p, q=label_q)
print command
print '\n'

os.system(command)



