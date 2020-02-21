import sys 
import os
import glob
import ROOT


def getOptions():
   from argparse import ArgumentParser

   parser = ArgumentParser(description='Script to merge the nanoAOD file resulting from a multijob production', add_help=True)
   parser.add_argument('--pl', type=str, dest='pl', help='label of the sample file = name of directory in pnfs', default='')

   return parser.parse_args()

if __name__ == "__main__":

   opt = getOptions()
   
   # first get the nanoAOD files
   locationSE = '/pnfs/psi.ch/cms/trivcat/store/user/anlyon/EcalProd/{a}/'.format(a=opt.pl)
   nanoName = 'step4_nj*.root'

   SEprefix = 'root://t3dcachedb.psi.ch:1094/'

   filesSE = [f for f in glob.glob(locationSE+nanoName)]

   listeSE = map(lambda x: SEprefix+x, filesSE)

   # create the outputdir that will contain the mergedNanoAOD file
   outputdir = '/pnfs/psi.ch/cms/trivcat/store/user/anlyon/JetMET_production/{a}/'.format(a=opt.pl)
   os.system('xrdfs t3dcachedb03.psi.ch mkdir {a}'.format(a=outputdir))

   # due to heavy protocols on the SE, proceed to the merging in the scratch
   workdir = '/scratch/anlyon/JetMET_production/{a}/'.format(a=opt.pl)
   os.system('mkdir {a}'.format(a=workdir))

   # copy the nanoAOD files in the workdir
   print 'Copying the files in the scratch'
   for fileName in listeSE:
      os.system('xrdcp {a} {b}'.format(a=fileName, b=workdir))

   print 'Start of the merge'
   filesWork = [f for f in glob.glob(workdir+nanoName)]
  
   command = 'python haddnano.py {a}/merged.root'.format(a=workdir)
   for fileName in filesWork:
      command = command + ' {a}'.format(a=fileName)

   os.system(command)

   # copying the file back to the storage element
   os.system('xrdcp {a}/merged.root {b}/{c}/merged.root'.format(a=workdir, b=SEprefix, c=outputdir))

   # empty the workdir
   os.system('rm -r {a}'.format(a=workdir))

   print 'Done'









