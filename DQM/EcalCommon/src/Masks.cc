// $Id: Masks.cc,v 1.72 2010/03/09 09:01:50 dellaric Exp $

/*!
  \file Masks.cc
  \brief Some "id" conversions
  \author B. Gobbo
  \version $Revision: 1.72 $
  \date $Date: 2010/03/09 09:01:50 $
*/

#include <sstream>
#include <iomanip>

#include "DQMServices/Core/interface/DQMStore.h"

#include <DataFormats/EcalDetId/interface/EBDetId.h>
#include <DataFormats/EcalDetId/interface/EEDetId.h>

#include <DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h>
#include <DataFormats/EcalDetId/interface/EcalScDetId.h>

#include "FWCore/Framework/interface/NoRecordException.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/DataRecord/interface/EcalDQMChannelStatusRcd.h"
#include "CondFormats/DataRecord/interface/EcalDQMTowerStatusRcd.h"

#include "DQM/EcalCommon/interface/UtilsClient.h"
#include "DQM/EcalCommon/interface/Numbers.h"

#include "DQM/EcalCommon/interface/Masks.h"

//-------------------------------------------------------------------------

const EcalDQMChannelStatus* Masks::channelStatus = 0;
const EcalDQMTowerStatus* Masks::towerStatus = 0;

bool Masks::init = false;

//-------------------------------------------------------------------------

void Masks::initMasking( const edm::EventSetup& setup, bool verbose ) {

  if( Masks::init ) return;

  if ( verbose ) std::cout << "Initializing EcalDQMChannelStatus/EcalDQMTowerStatus ..." << std::endl;

  Masks::init = true;

  if ( setup.find( edm::eventsetup::EventSetupRecordKey::makeKey< EcalDQMChannelStatusRcd >() ) ) {
    edm::ESHandle< EcalDQMChannelStatus > handle;
    setup.get< EcalDQMChannelStatusRcd >().get(handle);
    if ( handle.isValid() ) Masks::channelStatus = handle.product();
  }

  if ( setup.find( edm::eventsetup::EventSetupRecordKey::makeKey< EcalDQMTowerStatusRcd >() ) ) {
    edm::ESHandle< EcalDQMTowerStatus > handle;
    setup.get< EcalDQMTowerStatusRcd >().get(handle);
    if ( handle.isValid() ) Masks::towerStatus = handle.product();
  }

  if ( verbose ) std::cout << "done." << std::endl;

}

//-------------------------------------------------------------------------

bool Masks::maskChannel( int ism, int ix, int iy, uint32_t bits, const EcalSubdetector subdet ) throw( std::runtime_error ) {

  bool mask = false;

  if( subdet == EcalBarrel ) {

    EBDetId id(ism, (ix-1)+20*(iy-1)+1, EBDetId::SMCRYSTALMODE);
    if ( Masks::channelStatus ) {
      EcalDQMChannelStatus::const_iterator it = Masks::channelStatus->find( id.rawId() );
      if ( it != Masks::channelStatus->end() ) mask |= it->getStatusCode() & bits;
    }
    mask |= Masks::maskTower(ism, ix, iy, bits, EcalBarrel);

  } else if( subdet == EcalEndcap ) {

    int jx = ix + Numbers::ix0EE(ism);
    int jy = iy + Numbers::iy0EE(ism);

    if ( Numbers::validEE(ism, jx, jy) ) {
      EEDetId id(jx, jy, ism>=1&&ism<=9?-1:+1, EEDetId::XYMODE);
      if ( Masks::channelStatus ) {
        EcalDQMChannelStatus::const_iterator it = Masks::channelStatus->find( id.rawId() );
        if ( it != Masks::channelStatus->end() ) mask |= it->getStatusCode() & bits;
      }
      mask |= Masks::maskTower(ism, ix, iy, bits, EcalEndcap);
   }

  } else {

    std::ostringstream s;
    s << "Invalid subdetector: subdet = " << subdet;
    throw( std::runtime_error( s.str() ) );

  }

  return ( mask );

}

//-------------------------------------------------------------------------

bool Masks::maskTower( int ism, int ix, int iy, uint32_t bits, const EcalSubdetector subdet ) throw( std::runtime_error ) {

  bool mask = false;

  if( subdet == EcalBarrel ) {

    EBDetId id(ism, (ix-1)+20*(iy-1)+1, EBDetId::SMCRYSTALMODE);
    if ( towerStatus ) {
      EcalDQMTowerStatus::const_iterator it = Masks::towerStatus->find( id.tower().rawId() );
      if ( it != Masks::towerStatus->end() ) mask |= it->getStatusCode() & bits;
    }

  } else if( subdet == EcalEndcap ) {

    int jx = ix + Numbers::ix0EE(ism);
    int jy = iy + Numbers::iy0EE(ism);

    if ( Numbers::validEE(ism, jx, jy) ) {
      EEDetId id(jx, jy, ism>=1&&ism<=9?-1:+1, EEDetId::XYMODE);
      if ( Masks::towerStatus ) {
        EcalDQMTowerStatus::const_iterator it = Masks::towerStatus->find( id.sc().rawId() );
        if ( it != Masks::towerStatus->end() ) mask |= it->getStatusCode() & bits;
      }
   }

  } else {

    std::ostringstream s;
    s << "Invalid subdetector: subdet = " << subdet;
    throw( std::runtime_error( s.str() ) );

  }

  return ( mask );

}

//-------------------------------------------------------------------------

