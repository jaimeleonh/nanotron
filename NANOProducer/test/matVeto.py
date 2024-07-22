import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from Configuration.AlCa.GlobalTag import GlobalTag
process = cms.Process('matveto')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        "/store/data/Run2018D/ParkingBPH1/MINIAOD/UL2018_MiniAODv2-v1/2530000/04990C43-91C8-CE4D-B96F-49A93216E90C.root",
    )
)

process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v37', '')

process.matveto = cms.EDProducer("HitMaker",
    measurementTrackerEventInputTag = cms.InputTag("MeasurementTrackerEvent"),
)

from RecoTracker.MeasurementDet.measurementTrackerEventDefault_cfi import measurementTrackerEventDefault as _measurementTrackerEventDefault
process.MeasurementTrackerEvent = _measurementTrackerEventDefault.clone()

process.ana = cms.Path(process.matveto)
# process.schedule = cms.Schedule(process.ana)
