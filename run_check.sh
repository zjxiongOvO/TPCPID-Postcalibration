# filename="LHC22o_pass2_triggersel_small" 
# filename="LHC22o_pass2_medium"
# filename="LHC22p_pass2"
# filename="LHC22r_pass2"
# filename="LHC22t_pass2"

#############################################
root -l -b -q 'code/draw_dimdependence_check.cxx("'$filename'")'