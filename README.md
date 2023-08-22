# nanotron

![build tests](https://travis-ci.org/mieskolainen/nanotron.svg?branch=master)

Version 0.02

Based on a fusion of code from:

https://github.com/LLPDNNX/LLPReco

https://github.com/DiElectronX/BParkingNANO

</br>

Initialize the environment
```
source /cvmfs/cms.cern.ch/cmsset_default.sh
```

Initialize GRID
```
source /vols/grid/cms/setup.sh
voms-proxy-init --voms cms
```

Initialize CMSSW
```
scram list CMSSW

cmsrel CMSSW_13_0_10
cd CMSSW_13_0_10/src

cmsenv
```

Pull the repo
```
git clone git@github.com:jaimeleonh/nanotron.git nanotron -b 2023
```

Update the repo
```
cd nanotron/ && git pull origin main && cd ..
```

Build the code
```
scram build clean
scram build
```

Produce a custom NanoAOD tree
```
cmsRun nanotron/NANOProducer/test/produceNANO.py inputFiles=https://github.com/LLPDNNX/test-files/raw/master/miniaod/HNL_miniaod18.root year=2018 test=True isData=False

cmsRun nanotron/NANOProducer/test/produceNANO.py inputFiles=/store/mc/RunIIAutumn18MiniAOD/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v3/100000/005594DA-4AA0-3E48-A8C2-46DECDE2E925.root year=2018 test=True isData=False addSignalLHE=False

cmsRun nanotron/NANOProducer/test/produceNANO.py inputFiles=/store/data/Run2018B/ParkingBPH1/MINIAOD/05May2019-v2/230000/00496A25-08B6-FB4E-9681-D5FF4E1BE81F.root year=2018 test=True isData=True

cmsRun nanotron/NANOProducer/test/produceNANO.py inputFiles=/store/data/Run2023C/ParkingLLP/MINIAOD/PromptReco-v3/000/367/661/00000/0417d706-ded1-4f84-862f-f6a27f6c501e.root year=2023 isData=True

```

Test the custom NanoAOD tree (TBD)
```
python nanotron/test/check_tree.py
```


m.mieskolainen@imperial.ac.uk, 2023
