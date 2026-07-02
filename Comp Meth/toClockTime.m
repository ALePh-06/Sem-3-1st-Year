function timeStr = toClockTime(minutesElapsed, startHour)
  totalMinutes = mod(startHour*60 + minutesElapsed, 1440);  % wraps past midnight if needed
  hh = floor(totalMinutes / 60);
  mm = round(mod(totalMinutes, 60));
  if mm == 60                  % handle rounding edge case (e.g. 59.6 -> 60)
    mm = 0;
    hh = mod(hh + 1, 24);
  end
  timeStr = sprintf('%02d:%02d', hh, mm);
endfunction
