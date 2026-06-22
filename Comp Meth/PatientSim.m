function PatientSim(show, improve, patients, patientsPH, patientsNH)
  startHour = 9;

  % --- Group assignment per shift ---
  [patientsGP, patientsYP, patientsRP] = assignGroups(patientsPH);
  [patientsGN, patientsYN, patientsRN] = assignGroups(patientsNH);

  % --- Generate arrival/service times per zone per shift ---
  [arrGP, servGP] = exponentdist(patientsGP, 20,  10,  30,  8);
  [arrYP, servYP] = exponentdist(patientsYP, 45,  20,  70,  8);
  [arrRP, servRP] = exponentdist(patientsRP, 120, 10,  960, 8);

  [arrGN, servGN] = exponentdist(patientsGN, 20,  10,  30,  16);
  [arrYN, servYN] = exponentdist(patientsYN, 45,  20,  70,  16);
  [arrRN, servRN] = exponentdist(patientsRN, 120, 10,  960, 16);

  % --- Offset non-peak arrivals to start at 5pm (480 min from 9am) ---
  arrGN = arrGN + 480;
  arrYN = arrYN + 480;
  arrRN = arrRN + 480;

  % --- Merge into one shared pool ---
  arrR = [arrRP; arrRN];   servR = [servRP; servRN];
  arrY = [arrYP; arrYN];   servY = [servYP; servYN];
  arrG = [arrGP; arrGN];   servG = [servGP; servGN];

  arrivalTimes = [arrR; arrY; arrG];
  serviceTimes = [servR; servY; servG];
  totalPatients = length(arrivalTimes);
  priority = [ones(length(arrR),1); 2*ones(length(arrY),1); 3*ones(length(arrG),1)];

  % --- Simulate ---
  [startTime, finishTime, waitTime, doctorAssigned, overflow] = ...
      simulateQueue(arrivalTimes, serviceTimes, priority, improve);

  % --- Print results ---
  splitResultsByGroup(arrivalTimes, startTime, finishTime, waitTime, priority, ...
                      startHour, doctorAssigned, totalPatients, overflow, show, improve);
endfunction
