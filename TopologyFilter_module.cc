////////////////////////////////////////////////////////////////////////
// Class:       TopologyFilter
// Plugin Type: filter (art v2_07_03)
// File:        TopologyFilter_module.cc
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

#include <memory>

namespace cczeropi {
  class TopologyFilter;
}


class cczeropi::TopologyFilter : public art::EDFilter {
public:
  explicit TopologyFilter(fhicl::ParameterSet const & p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  TopologyFilter(TopologyFilter const &) = delete;
  TopologyFilter(TopologyFilter &&) = delete;
  TopologyFilter & operator = (TopologyFilter const &) = delete;
  TopologyFilter & operator = (TopologyFilter &&) = delete;

  // Required functions.
  bool filter(art::Event & e) override;
  void reconfigure(fhicl::ParameterSet const & p) override;
private:

  // Declare member data here.
  std::map< std::vector< int >, int > m_selection;
  float m_detectorHalfLengthX;
  float m_detectorHalfLengthY;
  float m_detectorHalfLengthZ;
  float m_coordinateOffsetX;
  float m_coordinateOffsetY;
  float m_coordinateOffsetZ;
  float m_selectedBorderX;
  float m_selectedBorderY;
  float m_selectedBorderZ;

};


cczeropi::TopologyFilter::TopologyFilter(fhicl::ParameterSet const & p)
// :
// Initialize member data here.
{
  // Call appropriate produces<>() functions here.
  
  this->reconfigure(p);
}

bool cczeropi::TopologyFilter::filter(art::Event & e)
{
  // Implementation of required member function here.

  typedef::std::map< std::vector< int >, int > topology_map;

  // Loop over everything and check it's working  
  // Show the chosen topology to filter on
  std::cout << "Filtering out topology:" << std::endl;
  for ( topology_map::iterator it = m_selection.begin(); it != m_selection.end(); ++it ) {
    std::vector< int > pdgVect = it->first;
    int                count   = it->second;
    
    std::cout << "  " << count << " particles with PDG in [";
    for ( int pdg : pdgVect ) {
        std::cout << " " << pdg << " ";
    }
    std::cout << "]" << std::endl;

  }

  // Get the MCTruth information 
  art::Handle< std::vector< simb::MCTruth > > mct_handle;
  e.getByLabel("generator", mct_handle ); 
    
  int size = mct_handle->size();
  
  if(mct_handle.isValid() && size) {
    // Loop over the truth info
    for(auto const& mct : (*mct_handle)) {

      // Check the neutrino came from the beam
      if(mct.Origin() != simb::kBeamNeutrino) continue;

      //-----------------------------------------------------------
      // Check the neutrino vertex is within the fiducial volume
      //-----------------------------------------------------------
      float nuVtxX = mct.GetNeutrino().Nu().Vx();                                                                      
      float nuVtxY = mct.GetNeutrino().Nu().Vy();                                                                             
      float nuVtxZ = mct.GetNeutrino().Nu().Vz(); 


      if ((nuVtxX > (m_detectorHalfLengthX - m_coordinateOffsetX - m_selectedBorderX)) || (nuVtxX < (-m_coordinateOffsetX + m_selectedBorderX)) ||
          (nuVtxY > (m_detectorHalfLengthY - m_coordinateOffsetY - m_selectedBorderY)) || (nuVtxY < (-m_coordinateOffsetY + m_selectedBorderY)) ||
          (nuVtxZ > (m_detectorHalfLengthZ - m_coordinateOffsetZ - m_selectedBorderZ)) || (nuVtxZ < (-m_coordinateOffsetZ + m_selectedBorderZ))) return false;

      //-----------------------------------------------------------
      //            Check the interaction
      //-----------------------------------------------------------
      
      // Get the number of particles
      const int n_particles = mct.NParticles();
      std::cout << " Number of particles in the event : " << n_particles << std::endl;
      
      // Loop over the map and get the elements
      for( topology_map::iterator it = m_selection.begin(); it != m_selection.end(); ++it ){

        std::vector< int > pdgVect = it->first;
        int                count   = it->second;
    
        // Initialise a counter for the number within the mct vector
        int particle_counter = 0;

        // Loop over the particles in the event
        for( int i = 0; i < n_particles; ++i ){
        
          // Find if the pdg code of the current particle is one of the ones in the map
          if( std::find( pdgVect.begin(), pdgVect.end(), mct.GetParticle(i).PdgCode() ) != pdgVect.end() ) ++particle_counter;

        }

        // If the counters don't match, return true
        if( particle_counter != count ){
          
          std::cout << " PDG codes of particles before filtering : " << std::endl;
        
          // Loop over the particles in the event
          for( int i = 0; i < n_particles; ++i ){
        
            std::cout << "           " << mct.GetParticle(i).PdgCode() << std::endl;

          }

          return false;

        }
      }
      
      std::cout << " Passed " << std::endl;
      std::cout << "nuVtxX=" << nuVtxX << " , nuVtxY = " << nuVtxY << " , nuVtxZ = " << nuVtxZ << std::endl;

    }
  }

  return true;

}
void cczeropi::TopologyFilter::reconfigure(fhicl::ParameterSet const & p) 
{
  std::vector< int > blankVect;
  std::vector< std::vector< int > > input;

  std::vector< int > selection1 = p.get< std::vector< int > >("Selection1", blankVect);   
  if ( selection1.size() != 0 ) input.push_back(selection1);

  std::vector< int > selection2 = p.get< std::vector< int > >("Selection2", blankVect);   
  if ( selection2.size() != 0 ) input.push_back(selection2);

  std::vector< int > selection3 = p.get< std::vector< int > >("Selection3", blankVect);   
  if ( selection3.size() != 0 ) input.push_back(selection3);


  for ( auto & inputVect : input ) {
    if ( inputVect.size() < 2 ) {
      std::cerr << " Error: Selection vector must have at least 2 elements " << std::endl;
      std::cerr << "        First element:     Number of particles of PDG code(s) specified " << std::endl;
      std::cerr << "        Remaining element: PDG codes to filter on " << std::endl;
      exit(1);
    }

    int count = inputVect[0];
    inputVect.erase( inputVect.begin() );

    m_selection.insert( std::make_pair( inputVect, count ) );
  }

  m_detectorHalfLengthX = p.get<float>("DetectorHalfLengthX");
  m_detectorHalfLengthY = p.get<float>("DetectorHalfLengthY");
  m_detectorHalfLengthZ = p.get<float>("DetectorHalfLengthZ");
  m_coordinateOffsetX   = p.get<float>("CoordinateOffsetX");
  m_coordinateOffsetY   = p.get<float>("CoordinateOffsetY");
  m_coordinateOffsetZ   = p.get<float>("CoordinateOffsetZ");
  m_selectedBorderX     = p.get<float>("SelectedBorderX");
  m_selectedBorderY     = p.get<float>("SelectedBorderY");
  m_selectedBorderZ     = p.get<float>("SelectedBorderZ");

}

DEFINE_ART_MODULE(cczeropi::TopologyFilter)
