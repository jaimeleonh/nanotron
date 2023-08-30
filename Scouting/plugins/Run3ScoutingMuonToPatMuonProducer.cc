// Run3ScoutingMuon to pat::Muon producer

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Scouting/interface/Run3ScoutingMuon.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "boost/algorithm/string.hpp"

#include <vector>
#include <math.h>
#include <unordered_map>
#include <iostream>
#include <regex>

class Run3ScoutingMuonToPatMuonProducer: public edm::stream::EDProducer<>
{
    public:
        Run3ScoutingMuonToPatMuonProducer(const edm::ParameterSet &iConfig);
        ~Run3ScoutingMuonToPatMuonProducer() override;
        void produce(edm::Event &iEvent, const edm::EventSetup &iSetup) override;

        static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

    private:
        edm::EDGetTokenT<std::vector<Run3ScoutingMuon>> muonToken_;
        edm::EDGetTokenT<std::vector<reco::Track>> trackToken_;
};

Run3ScoutingMuonToPatMuonProducer::Run3ScoutingMuonToPatMuonProducer(const edm::ParameterSet &iConfig) {
    muonToken_ = consumes<std::vector<Run3ScoutingMuon>>(iConfig.getParameter<edm::InputTag>("muonSource"));
    trackToken_ = consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("trackSource"));
    produces<std::vector<pat::Muon>>();
    produces<edm::ValueMap<int> >().setBranchAlias("numberofmatchedstations");
}

Run3ScoutingMuonToPatMuonProducer::~Run3ScoutingMuonToPatMuonProducer() {}

void Run3ScoutingMuonToPatMuonProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) {
    edm::Handle<std::vector<Run3ScoutingMuon>> muons;
    iEvent.getByToken(muonToken_, muons);
    edm::Handle<std::vector<reco::Track>> tracks;
    iEvent.getByToken(trackToken_, tracks);

    auto patMuons = std::make_unique<std::vector<pat::Muon>>();
    std::vector<int> numberofmatchedstations;
    numberofmatchedstations.reserve(muons->size());
    // for (const auto &muon: *muons) {
    for (unsigned int imuon = 0; imuon < muons->size(); imuon++) {
        auto muon = muons->at(imuon);
        pat::Muon patmuon;
        math::PtEtaPhiMLorentzVectorD lv(muon.pt(), muon.eta(), muon.phi(), muon.m());
        patmuon.setP4(lv);
        patmuon.setCharge(muon.charge());
        patmuon.setType(muon.type());
        patmuon.setDB(muon.trk_dxy(), muon.trk_dxyError(), pat::Muon::PV2D);
        patmuon.setDB(muon.trk_dz(), muon.trk_dzError(), pat::Muon::PVDZ);

        numberofmatchedstations.push_back(muon.nRecoMuonMatchedStations());


        // std::cout << "===========================" << std::endl;
        // std::cout << muon.trk_vx() << " ";
        // std::cout << muon.trk_vy() << " ";
        // std::cout << muon.trk_vz() << std::endl;
        // std::cout << "===========================" << std::endl;

        // for (unsigned int itrack = 0; itrack < tracks->size(); itrack++) {
            // std::cout << tracks->at(itrack).vx() << " ";
            // std::cout << tracks->at(itrack).vy() << " ";
            // std::cout << tracks->at(itrack).vz() << std::endl;
        // }

        // track
        // reco::Track::Point v(muon.trk_vx(), muon.trk_vy(), muon.trk_vz());
        // reco::Track::Vector p(
            // muon.trk_pt() * cos(muon.trk_phi()),
            // muon.trk_pt() * sin(muon.trk_phi()),
            // muon.trk_pt() * sinh(muon.trk_eta())
        // );
        // reco::TrackBase::CovarianceMatrix cov(5);
        // cov(0, 1) = muon.trk_qoverp_lambda_cov();
        // cov(0, 2) = muon.trk_qoverp_phi_cov();
        // cov(0, 3) = muon.trk_qoverp_dxy_cov();
        // cov(0, 4) = muon.trk_qoverp_dsz_cov();
        // cov(1, 2) = muon.trk_lambda_phi_cov();
        // cov(1, 3) = muon.trk_lambda_dxy_cov();
        // cov(1, 4) = muon.trk_lambda_dsz_cov();
        // cov(2, 3) = muon.trk_phi_dxy_cov();
        // cov(2, 4) = muon.trk_phi_dsz_cov();
        // cov(3, 4) = muon.trk_dxy_dsz_cov();
        // reco::Track trk(muon.trk_chi2(), muon.trk_ndof(), v, p, -1, cov);
        // reco::TrackCollection trkCol({trk});
        edm::Ref<reco::TrackCollection> myRefTrack(tracks, imuon);
        patmuon.setTrack(myRefTrack);
        patmuon.setBestTrack(reco::Muon::MuonTrackType::InnerTrack);
        patMuons->push_back(patmuon);
    }
    auto coll = iEvent.put(std::move(patMuons));
    auto out = std::make_unique<edm::ValueMap<int>>();
    edm::ValueMap<int>::Filler filler(*out);
    filler.insert(coll, numberofmatchedstations.begin(), numberofmatchedstations.end());
    filler.fill();
    iEvent.put(std::move(out));
}

void Run3ScoutingMuonToPatMuonProducer::fillDescriptions(edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription iDesc;
  iDesc.setComment("Run3ScoutingMuon to pat::Muon producer module");

  // input source
  iDesc.add<edm::InputTag>("muonSource", edm::InputTag("no default"))->setComment("input collection");
  iDesc.add<edm::InputTag>("trackSource", edm::InputTag("no default"))->setComment("input collection");

  descriptions.add("Run3ScoutingMuonToPatMuonProducer", iDesc);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(Run3ScoutingMuonToPatMuonProducer);

