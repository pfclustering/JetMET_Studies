import sys 
import os
import glob
import ROOT
from argparse import ArgumentParser

# get the parsed arguments
parser = ArgumentParser(description='Script to merge the nanoAOD file resulting from a multijob production', add_help=True)
parser.add_argument('--i1', type=str, dest='i1', help='file info 1', default='JetHT')
parser.add_argument('--i2', type=str, dest='i2', help='file info 2', default='Run2016F')
parser.add_argument('--i3', type=str, dest='i3', help='file info 3', default='ForValUL2016-v1')
options = parser.parse_args()


fileDir = '/pnfs/psi.ch/cms/trivcat/store/user/anlyon/JetMET_production/{a}_{b}-{c}/'.format(a=options.i1, b=options.i2, c=options.i3)

# read the "liste" file created in the analyzer_from_root.sh script
listeFile = open('liste')
lines_file = listeFile.readlines()

# create a list of the dataset names
datasetName = []

for line in lines_file:
   index = line.rfind('/')
   index_f = line.rfind('root')
   datasetName.append(line[index+1:index_f+4])

# and creates new one with only valid files
newFile = open("fileListe", "w+")

for iFile in datasetName:
   if(ROOT.TFile.Open("root://t3dcachedb.psi.ch:1094/{a}/{b}/file.root".format(a=fileDir, b=iFile))):
      newFile.write('{a}\n'.format(a=iFile))
   else:
      print 'file not found - will not be analysed'
  












