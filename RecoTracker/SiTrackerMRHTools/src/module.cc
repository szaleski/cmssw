
#include "RecoTracker/SiTrackerMRHTools/interface/SiTrackerMultiRecHitUpdator.h"
#include "RecoTracker/SiTrackerMRHTools/interface/SiTrackerMultiRecHitUpdatorMTF.h"
#include "RecoTracker/SiTrackerMRHTools/interface/MultiRecHitCollector.h"
#include "RecoTracker/SiTrackerMRHTools/interface/MultiTrackFilterHitCollector.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/eventsetupdata_registration_macro.h"
#include "FWCore/Framework/interface/eventsetuprecord_registration_macro.h"

EVENTSETUP_DATA_REG(SiTrackerMultiRecHitUpdator);
EVENTSETUP_DATA_REG(SiTrackerMultiRecHitUpdatorMTF);
EVENTSETUP_DATA_REG(MultiRecHitCollector);
EVENTSETUP_DATA_REG(MultiTrackFilterHitCollector);
