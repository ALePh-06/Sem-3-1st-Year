function trackShiftOverflow(startTime)
  shiftEnd      = 960;    % 9am next day (end of 16-hr shift)
  nextPeakStart = 1440;   % 5pm next day (24-hr mark)

  withinShift      = sum(startTime <= shiftEnd);
  nonPeakOverflow  = sum(startTime > shiftEnd & startTime <= nextPeakStart);
  nextPeakOverflow = sum(startTime > nextPeakStart);

  printf("----------------------------------------\n");
  printf("Shift Overflow Tracking\n");
  printf("Started within shift (before 9am)   : %d patients\n", withinShift);
  printf("Started after 9am, before next 5pm  : %d patients\n", nonPeakOverflow);
  printf("Started after next day's 5pm        : %d patients\n", nextPeakOverflow);
  printf("----------------------------------------\n");
endfunction
