function [patientsG, patientsY, patientsR] = assignGroups(patients)
  U = rand(patients, 1);
  groups = repmat('G', patients, 1);
  groups(U >= 0.7 & U < 0.95) = 'Y';
  groups(U >= 0.95) = 'R';

  patientsG = sum(groups == 'G');
  patientsY = sum(groups == 'Y');
  patientsR = sum(groups == 'R');

endfunction
