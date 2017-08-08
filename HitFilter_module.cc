////////////////////////////////////////////////////////////////////////
// Class:       HitFilter
// Plugin Type: filter (art v2_07_03)
// File:        HitFilter_module.cc
//
// Generated at Wed Jul  5 17:37:31 2017 by Rhiannon Jones using cetskelgen
// from cetlib version v3_00_01.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "lardataobj/RecoBase/Hit.h"

#include <memory>

namespace cczeropi {
  class HitFilter;
}


class cczeropi::HitFilter : public art::EDFilter {
public:
  explicit HitFilter(fhicl::ParameterSet const & p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  HitFilter(HitFilter const &) = delete;
  HitFilter(HitFilter &&) = delete;
  HitFilter & operator = (HitFilter const &) = delete;
  HitFilter & operator = (HitFilter &&) = delete;

  // Required functions.
  bool filter(art::Event & e) override;
  void reconfigure(fhicl::ParameterSet const & p) override;
private:

  // Declare member data here.
  float m_minHitAssociations;

};


cczeropi::HitFilter::HitFilter(fhicl::ParameterSet const & p)
// :
// Initialize member data here.
{
  // Call appropriate produces<>() functions here.
  
  this->reconfigure(p);
}

bool cczeropi::HitFilter::filter(art::Event & e)
{
  // Implementation of required member function here.
  // Get the MCTruth information 
  art::Handle< std::vector< recob::Hit > > hit_handle;
  e.getByLabel("gaushit", hit_handle ); 
  
  int size = hit_handle->size();

  if(hit_handle.isValid() && size ) {

    std::cout << " Number of hits : " << size << std::endl;

    if( size <= m_minHitAssociations ) return false;

  }

  return true;

}
void cczeropi::HitFilter::reconfigure(fhicl::ParameterSet const & p) 
{

    m_minHitAssociations = p.get<float>("MinHitAssociations");

}

DEFINE_ART_MODULE(cczeropi::HitFilter)
