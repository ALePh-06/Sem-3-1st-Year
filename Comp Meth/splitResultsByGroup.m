function splitResultsByGroup(arrivalTimes, startTime, finishTime, waitTime, priority, startHour, doctorAssigned, totalPatients, overflow, show, improve)
  % Filter displayed table by peak/non-peak based on 'show'
  if show == 1
    showMask = (startTime >= 0 & startTime < 480);      % Peak Hours (9am-5pm)
  else
    showMask = (startTime >= 480 & startTime < 1440);   % Non-Peak Hours (5pm-9am)
  endif
  % Red zone (priority == 1)
  idxR = (priority == 1) & ~overflow & showMask;
  printf("Red Zone\n");
  PatientResults(arrivalTimes(idxR), startTime(idxR), finishTime(idxR), waitTime(idxR), startHour, doctorAssigned(idxR));
  % Yellow zone (priority == 2)
  idxY = (priority == 2) & ~overflow & showMask;
  printf("Yellow Zone\n");
  PatientResults(arrivalTimes(idxY), startTime(idxY), finishTime(idxY), waitTime(idxY), startHour, doctorAssigned(idxY));
  % Green zone (priority == 3)
  idxG = (priority == 3) & ~overflow & showMask;
  printf("Green Zone\n");
  PatientResults(arrivalTimes(idxG), startTime(idxG), finishTime(idxG), waitTime(idxG), startHour, doctorAssigned(idxG));
  printf("========================================\n");
  printf("Overall Statistics\n");
  served = ~overflow;
  % Guard against empty served set
  if any(served)
    avgWait    = mean(waitTime(served));
    avgService = mean(finishTime(served) - startTime(served));
    T = max(arrivalTimes) - min(arrivalTimes);
    lambda = totalPatients / T;
    avgQueueLength = lambda * avgWait;
    printf("Average Waiting Time : %.2f min\n", avgWait);
    printf("Average Service Time : %.2f min\n", avgService);
    printf("Average Queue Length : %.2f patients\n", avgQueueLength);
  else
    printf("No patients were served.\n");
  endif
  reportShiftBreakdown(startTime, overflow);
  % --- Per-doctor utilization ---
  printf("Doctor Utilization\n");
  if improve == 1
    dayDoctors   = 7;
    nightDoctors = 5;
  else
    dayDoctors   = 5;
    nightDoctors = 3;
  endif
  totalDoctors = dayDoctors + nightDoctors;
  for d = 1:totalDoctors
    if d == 1
      printf("Doctors that work in the Peak Hours\n");
    elseif d == dayDoctors + 1
      printf("Doctors that work in the Non-Peak Hours\n");
    endif
    if d <= dayDoctors
      shiftLength = 8*60;
    else
      shiftLength = 16*60;
    endif
    idx = (doctorAssigned == d) & served;     % exclude overflow from busy time
    if any(idx)
      busyTime = sum(finishTime(idx) - startTime(idx));
    else
      busyTime = 0;
    endif
    util = (busyTime / shiftLength) * 100;
    printf("Doctor %d : %.2f %%\n", d, util);
  endfor
  printf("========================================\n");
endfunction
