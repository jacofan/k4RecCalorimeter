// k4FWCore / k4Interface
#include "k4FWCore/Transformer.h"

// EDM4hep
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/ClusterCollection.h"

// STL
#include <vector>

struct TRAPPISTTool final 
    : k4FWCore::MultiTransformer<
        std::tuple<
            edm4hep::CalorimeterHitCollection,
            edm4hep::ReconstructedParticleCollection
        >(
            const edm4hep::ClusterCollection&
        )
    >
{
public:
    TRAPPISTTool(const std::string& name, ISvcLocator* svcLoc)
        : MultiTransformer(
            name,
            svcLoc,
            {
                KeyValues("Clusters", {"UnpairedClusters"})
            },
            {
                KeyValues("Pi0Hits", {"DefaultOutputCollectionName1"}),
                KeyValues("Pi0Particles", {"DefaultOutputCollectionName2"})
            }
        )
        {}
    
    StatusCode initialize() override {
        // Initialize the tool, onnx, model, etc.
        return StatusCode::SUCCESS;
    }

    std::tuple <edm4hep::CalorimeterHitCollection, 
                edm4hep::ReconstructedParticleCollection> 
    operator()(const edm4hep::ClusterCollection& unpairedClusters) const override {
        // body of the tool
        std::vector<Hit4D> hitsData; // input for the GATr classifier
        //hitsData.reserve(clusteredHits.size());
        for (const auto& cluster : unpairedClusters) {
            for (const auto& hit : cluster.getHits()) {
                const auto& pos = hit.getPosition();
                hitsData.push_back({pos.x, pos.y, pos.z, hit.getEnergy()});
            }
        }
        edm4hep::CalorimeterHitCollection pi0Hits;
        edm4hep::ReconstructedParticleCollection pi0Particles;

    return std::make_tuple(std::move(pi0Hits), std::move(pi0Particles));

    }



    StatusCode finalize() override {
        // Finalize the tool, write report, etc.
        return StatusCode::SUCCESS;
    }


private:
    struct Hit4D {
        float x;
        float y;
        float z;
        float e;
    };

};

DECLARE_COMPONENT(TRAPPISTTool)