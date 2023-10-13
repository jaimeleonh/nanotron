#for s in scenarioB1_mpi_4_mA_1p33_ctau_10 scenarioB2_mpi_4_mA_2p10_ctau_10 scenarioC_mpi_10_mA_8p00_ctau_10 scenarioA_mpi_10_mA_3p33_ctau_10
for s in scenarioA_mpi_10_mA_3p33_ctau_100
do
    for i in {1..10}
    do
        echo "cd /home/hep/jleonhol/samplegeneration/CMSSW_10_6_26/src" > j_${s}_${i}.sh
        echo "cmsenv" >> j_${s}_${i}.sh
        echo "cmsRun nanotron/NANOProducer/test/produceNANO_auto.py isData=False inputFiles=file:/vols/cms/jleonhol/samples/UL2018/${s}/miniAOD_${i}.root filename=${s} number=${i}" >> j_${s}_${i}.sh
        chmod +x j_${s}_${i}.sh
        # qsub -q hep.q -l h_rt=0:5:0 j_${s}_${i}.sh
    done
done
