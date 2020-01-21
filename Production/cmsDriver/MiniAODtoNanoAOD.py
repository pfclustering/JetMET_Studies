# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step5 --conditions 106X_dataRun2_v15 --era Run2_2017 --eventcontent NANOAODSIM --filein root://cms-xrd-global.cern.ch//store/relval/CMSSW_10_6_1/JetHT/MINIAOD/106X_dataRun2_v15_rsb_RelVal_2017C-v1/10000/DF91AEDE-0FC0-DB42-9037-8CABDB66F4CE.root -s NANO --datatier NANOAODSIM --geometry DB:Extended --python NanoData.py --no_exec --fileout file:data.root -n 10000 --data
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2016_cff import Run2_2016
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
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.nevents)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/relval/CMSSW_10_6_1/JetHT/MINIAOD/106X_dataRun2_v15_rsb_RelVal_2017C-v1/10000/DF91AEDE-0FC0-DB42-9037-8CABDB66F4CE.root'),
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch/{a}'.format(a=options.productionName)),
    secondaryFileNames = cms.untracked.vstring()

)

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')  #so that the code still runs if product not found
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step5 nevts:10000'),
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
    fileName = cms.untracked.string('file:file.root'),
    outputCommands = process.NANOAODSIMEventContent.outputCommands
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v25', '')

# Path and EndPath definitions
process.nanoAOD_step = cms.Path(process.nanoSequence)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeData 

#call to customisation function nanoAOD_customizeData imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeData(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
