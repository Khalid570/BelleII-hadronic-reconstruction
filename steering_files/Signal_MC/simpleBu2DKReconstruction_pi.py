#!/usr/bin/env python3
import basf2 as b2
import modularAnalysis as ma
import variables.collections as vc
import variables.utils as vu
import vertex

# create path
main = b2.Path()

# load input data from mdst/udst file
ma.inputMdstList(
    environmentType="default",
    filelist=["~khalid/sad14/my_mdst_output_piiiizzz.root"],
    path=main
)

# fill final state particle lists
ma.fillParticleList(
    "K+:all",
    cut="",
    path=main
)

ma.fillParticleList(
    "pi+:all",
    cut="",
    path=main
)
ma.cutAndCopyList(
    'K+:cut',
    'K+:all',
    'kaonID >0.5',
    path=main
)
ma.cutAndCopyList(
    'pi+:cut',
    'pi+:all',
    "pionID >0.1",
    path=main
)

ma.cutAndCopyList(
    'pi+:cutt',
    'pi+:all',
    "pionID >0.1",
    path=main
)
ma.cutAndCopyList(
    'pi-:cuttt',
    'pi+:all',
    'pionID >0.1',
    path=main
)

# combine final state particles to form composite particles
ma.reconstructDecay(
    "D+:kpipi -> K-:cut pi+:cut pi+:cutt",
    cut="",
    path=main
)

# combine D+ with bachelor kaon to form B0 candidates
ma.reconstructDecay(
    "anti-B0 -> D+:kpipi pi-:cuttt",
    cut="Mbc > 5.2 and abs(deltaE) < 0.5",
    path=main
)
vertex.treeFit("anti-B0", massConstraint=["D+"], path=main)

# match reconstructed with MC particles
ma.matchMCTruth("anti-B0", path=main)
#ma.applyCuts("anti-B0", "isSignal > 0.5 ", path = main)

# perform best candidate selection
b2.set_random_seed("Belle II Summer School 2021")
ma.rankByHighest("anti-B0", variable='random', numBest=1, path=main)

# Create list of variables to save into the output file
b_vars = []

standard_vars = vc.kinematics + vc.mc_kinematics + vc.mc_truth

# Variables for final state particles (kaons, pions)
fs_vars = vc.pid + vc.track + vc.track_hits + standard_vars
b_vars += vu.create_aliases_for_selected(
    fs_vars,
    "anti-B0 -> [D+ -> ^K- ^pi+ ^pi+] ^pi-",
    prefix=["D_K", "D_pi", "D_pit", "pi"],     	
)

standard_vars += vc.vertex + vc.mc_vertex

# Variables for D+
D_vars = vc.inv_mass + standard_vars
b_vars += vu.create_aliases_for_selected(
    D_vars,
    "anti-B0 -> ^D+ pi-"
)

# Variables for B0
b_vars += vc.deltae_mbc + standard_vars

# Save variables to an output file (ntuple)
ma.variablesToNtuple(
    "anti-B0",
    variables=b_vars,
    filename="Bu2DK_pi.root",
    treename="tree",
    path=main
)

# Start the event loop (actually start processing things)
b2.process(main)

# print out the summary
print(b2.statistics)
