# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step5 --conditions 106X_dataRun2_v15 --era Run2_2017 --eventcontent NANOAODSIM --filein root://cms-xrd-global.cern.ch//store/relval/CMSSW_10_6_0_pre4/RelValQCD_FlatPt_15_3000HS_13/MINIAODSIM/PU25ns_106X_upgrade2018_realistic_v4-v1/10000/F466F220-316A-AF4E-B986-C1700BCB4F16.root -s NANO --datatier NANOAODSIM --geometry DB:Extended --io NanoFull_2018PU.io --python NanoFull_2018PU.py --no_exec --fileout file:NoPU.root
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2016_cff import Run2_2016
#from Configuration.Eras.Era_Run2_2016_cff import Run2_2016
#from Configuration.Eras.Era_Run2_2017_cff import Run2_2017
from FWCore.ParameterSet.VarParsing import VarParsing


options = VarParsing('standard')

options.register('nevents',
                  -1,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                 "Number of events to process")

options.register('productionName',
                 '/store/relval/CMSSW_10_6_8/RelValQCD_FlatPt_15_3000HS_13/MINIAODSIM/PU25ns_106X_mcRun2_asymptotic_v9_postVFP-v2/20000/96ABF93D-CD98-AD4F-9A69-AC448627FE7A.root',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                 "Name of the production")

options.register('dataset',
                  '96ABF93D-CD98-AD4F-9A69-AC448627FE7A.root',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                 "Name of the dataset")

options.register('globalTag',
                  '106X_mcRun2_asymptotic_v9_postVFP',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                 "GlobalTag of the sample")



options.parseArguments()

process = cms.Process('NANO',Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(options.nevents)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/relval/CMSSW_10_6_0_pre4/RelValQCD_FlatPt_15_3000HS_13/MINIAODSIM/PU25ns_106X_upgrade2018_realistic_v4-v1/10000/F466F220-316A-AF4E-B986-C1700BCB4F16.root'),
    #fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/relval/CMSSW_10_6_8/RelValQCD_FlatPt_15_3000HS_13/MINIAODSIM/PU25ns_106X_mcRun2_asymptotic_v9_postVFP-v2/20000/C97347EF-9DF4-0D48-BC07-0BEEB326A724.root'),
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch/{a}'.format(a=options.productionName)),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
      SkipEvent = cms.untracked.vstring('ProductNotFound')  #so that the code still runs if product not found
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step5 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAODSIM'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file.root'),
    outputCommands = process.NANOAODSIMEventContent.outputCommands
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '106X_mcRun2_asymptotic_preVFP_v3', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '{a}'.format(a=options.globalTag), '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '106X_mcRun2_asymptotic_v9', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v25', '')

# Path and EndPath definitions
process.nanoAOD_step = cms.Path(process.nanoSequenceMC)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC 

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
