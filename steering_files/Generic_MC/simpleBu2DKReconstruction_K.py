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
    filelist='mdst.root',
    path=main,
)
goodTrack = 'abs(dz) < 2.0 and dr < 0.5 and pt > 0.1 and E < 5.5 and thetaInCDCAcceptance'
z = 'kaonID >0.5' + ' and ' + goodTrack
x = 'pionID >0.1' + ' and ' + goodTrack
# fill final state particle lists
ma.fillParticleList(
    "K+:all",
    cut="",
    path=main,
)

ma.fillParticleList(
    "pi+:all",
    cut="",
    path=main,
)
ma.cutAndCopyList(
    'K+:cut',
    'K+:all',
    z,
    path=main,
)
ma.cutAndCopyList(
    'K+:cutt',
    'K+:all',
    z,
    path=main,
)
ma.cutAndCopyList(
    'pi+:cut',
    'pi+:all',
    x,
    path=main,
)

ma.cutAndCopyList(
    'pi+:cutt',
    'pi+:all',
    x,
    path=main,
)

# combine final state particles to form composite particles
ma.reconstructDecay(
    "D+:kpipi -> K-:cut pi+:cut pi+:cutt",
    cut="M > 1.85839 and M < 1.88062",
    path=main,
)

# combine D+ with bachelor kaon to form B0 candidates
ma.reconstructDecay(
    "anti-B0:DK -> D+:kpipi K-:cutt",
    cut="Mbc > 5.27192 and Mbc < 5.28723 and abs(deltaE) < 0.5",
    path=main,
)

# Vertex fitting with a mass constraint
vertex.treeFit("anti-B0:DK",  massConstraint=["D+"], path=main)

#match reconstructed with MC particles
ma.matchMCTruth("anti-B0:DK", path=main)

# perform best candidate selection
b2.set_random_seed("m")
ma.rankByHighest("anti-B0:DK", variable='random', numBest=1, path=main)

# Create list of variables to save into the output file

# Variables for final state particles (kaons, pions)
fs_hadron_vars = vu.create_aliases_for_selected(
    list_of_variables= vc.kinematics + vc.mc_kinematics + vc.mc_truth + vc.pid + vc.track + vc.track_hits,
    decay_string='anti-B0 -> [D+ -> ^K- ^pi+ ^pi+] ^K-',
    prefix=["D_K", "D_pi", "D_pit", "K"],
)

# Variables for D+
D_vars = vu.create_aliases_for_selected(
    list_of_variables = vc.kinematics + vc.mc_kinematics + vc.mc_truth + vc.vertex + vc.mc_vertex + vc.inv_mass,
    decay_string="anti-B0 -> ^D+ K-",
)

B_vars = vu.create_aliases_for_selected(
    list_of_variables = vc.kinematics + vc.mc_kinematics + vc.mc_truth + vc.vertex + vc.mc_vertex + vc.deltae_mbc,
    decay_string="^anti-B0",
)

# Save variables to an output file (ntuple)
ma.variablesToNtuple(
    'anti-B0:DK',
    variables = fs_hadron_vars + D_vars + B_vars,
    filename='ntuple.root',
    treename='dsttree',
    path=main,
)

# Start the event loop (actually start processing things)
b2.process(main)
print(b2.statistics)
