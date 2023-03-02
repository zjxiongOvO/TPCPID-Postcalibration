# filename="LHC22m_pass2_subset"
# filename="LHC22f_pass2"
# filename="LHC22m_pass2"

# filename="LHC22q_pass2_triggersel" #
filename="LHC22r_pass2_lowIR" #good
# filename="LHC22t_pass2_triggersel" #ready but not good(NsigmaPr)
# filename="LHC22o_pass2_triggersel_small" #ready but not good(NsigmaPr) good!
# filename="LHC22o_pass2_medium" #ready but not good(NsigmaPr) bb not updated good!
# filename="LHC22o_pass2_small" #ready but not good(NsigmaPr)
# filename="LHC22r_pass2" #ready but not good(NsigmaPr) bb not updated good
# filename="LHC22p_pass2" #ready but not good(NsigmaPr) bb not updated good
# filename="LHC22t_pass2" #ready but not good(NsigmaPr) good
# filename="LHC22o_pass2_triggersel"






###############################################
ischeck="map"
# ischeck="check"
###############################################
# root -l -b -q 'code/bin_generater.cxx("'$filename'")'
# root -l -b -q 'code/v0check.cxx("'$filename'","'$ischeck'",1)'
# root -l -b -q 'code/postcalibration.cxx("'$filename'","'$ischeck'",1)'
root -l -b -q 'code/drawmap.cxx("'$filename'","'$ischeck'")'
# root -l -b -q 'code/get_dependence_dimbydim.cxx("'$filename'","'$ischeck'")'
# root -l -b -q 'code/draw_dimdependence.cxx("'$filename'","'$ischeck'")'
###############################################