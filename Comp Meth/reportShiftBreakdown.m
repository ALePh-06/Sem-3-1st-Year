function reportShiftBreakdown(startTime, overflow, patientsPH, patientsNH)
  peakHours     = sum(startTime >= 0   & startTime < 480  & ~isnan(startTime));
  nonPeakHours  = sum(startTime >= 480 & startTime < 1440 & ~isnan(startTime));
  overflowCount = sum(overflow);

  printf("----------------------------------------\n");
  printf("Shift Breakdown\n ");
  printf("Patients Received\n");
  printf("Peak Hours (9am-5pm) : %d patients\n", patientsPH);
  printf("Non-Peak Hours (5pm-9am)  : %d patients\n", patientsNH);
  printf("Patients Served\n");
  printf("Peak Hours (9am-5pm)      : %d patients\n", peakHours);
  printf("Non-Peak Hours (5pm-9am)  : %d patients\n", nonPeakHours);
  printf("Overflow (not served)     : %d patients\n", overflowCount);
  printf("----------------------------------------\n");
endfunction
