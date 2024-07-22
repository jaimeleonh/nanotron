#include "nanotron/NANOProducer/plugins/HitMaker.h" 
#include "TMath.h"

using namespace edm;
using namespace std;


HitMaker::HitMaker(const edm::ParameterSet& iConfig)
{
    measurementTrackerEventToken_ = consumes<MeasurementTrackerEvent>(iConfig.getParameter<InputTag>("measurementTrackerEventInputTag"));
}

HitMaker::~HitMaker(){
}

void HitMaker::beginJob(){}

void HitMaker::endJob(){}

void HitMaker::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup){}

void HitMaker::getMinDetDistance(const GeomDet *det){

  float xy[4][2];
  float dz;
  const Bounds *b = &((det->surface()).bounds());

  if (const TrapezoidalPlaneBounds *b2 = dynamic_cast<const TrapezoidalPlaneBounds *>(b)) {
    // See sec. "TrapezoidalPlaneBounds parameters" in doc/reco-geom-notes.txt
    std::array<const float, 4> const &par = b2->parameters();
    xy[0][0] = -par[0];
    xy[0][1] = -par[3];
    xy[1][0] = -par[1];
    xy[1][1] = par[3];
    xy[2][0] = par[1];
    xy[2][1] = par[3];
    xy[3][0] = par[0];
    xy[3][1] = -par[3];
    dz = par[2];
  } else if (const RectangularPlaneBounds *b2 = dynamic_cast<const RectangularPlaneBounds *>(b)) {
    // Rectangular
    float dx = b2->width() * 0.5;   // half width
    float dy = b2->length() * 0.5;  // half length
    xy[0][0] = -dx;
    xy[0][1] = -dy;
    xy[1][0] = -dx;
    xy[1][1] = dy;
    xy[2][0] = dx;
    xy[2][1] = dy;
    xy[3][0] = dx;
    xy[3][1] = -dy;
    dz = b2->thickness() * 0.5;  // half thickness
  }
  for (int i = 0; i < 4; ++i) {
    Local3DPoint lp1(xy[i][0], xy[i][1], -dz);
    Local3DPoint lp2(xy[i][0], xy[i][1], dz);
    
    GlobalPoint retPoint=det->surface().toGlobal(lp1);
    std::cout << retPoint.x() << " " << retPoint.y() << " " << retPoint.z() << std::endl;
    retPoint=det->surface().toGlobal(lp2);
    std::cout << retPoint.x() << " " << retPoint.y() << " " << retPoint.z() << std::endl;
 }
}

void HitMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

    bool debug = false;

    //measurementTracker_ = iSetup.getHandle(measurementTrackerToken_);
    //auto const& searchGeom = *(*measurementTracker_).geometricSearchTracker();
    
    edm::ESHandle<MeasurementTracker> measurementTrackerHandle;
    iSetup.get<CkfComponentsRecord>().get(measurementTrackerHandle);
    auto const& searchGeom = *(*measurementTrackerHandle).geometricSearchTracker();

    if (debug) {
        std::cout << std::endl;
        std::cout << "------- Run " << iEvent.id().run() << " Lumi " << iEvent.luminosityBlock() << " Event " << iEvent.id().event() << " -------" << std::endl;
    }

    // nlohmann::json j;


    for (auto const& detL : searchGeom.allLayers()){
        auto const& components = detL->basicComponents();
        for (auto const& comp: components){
            if(!comp->isLeaf()){
               for (auto const& subcomp: comp->components()){
                   getMinDetDistance(subcomp);
               }
            }
            else{
               getMinDetDistance(comp);
           }
        } 
    }
}

DEFINE_FWK_MODULE(HitMaker);
