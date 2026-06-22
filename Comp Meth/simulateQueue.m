function [startTime, finishTime, waitTime, doctorAssigned, overflow] = simulateQueue(arrivalTimes, serviceTimes, priority, improve)
  n = length(arrivalTimes);
  startTime      = nan(n,1);
  finishTime     = nan(n,1);
  doctorAssigned = zeros(n,1, 'int32');
  served         = false(n,1);
  overflow       = false(n,1);
  closeTime  = 1440;

  if improve == 1
    totalSlots = 7 + 5;     % improved: 7 day + 5 night
  else
    totalSlots = 5 + 3;     % normal: 5 day + 3 night
  endif
  doctorFreeAt = zeros(totalSlots,1);

  [sortedArrival, order] = sort(arrivalTimes);
  waitingQueue = [];
  nextArrivalPtr = 1;
  currentClock = 0;

  while any(~served & ~overflow)
    if nextArrivalPtr <= n
      nextArrivalTime = sortedArrival(nextArrivalPtr);
    else
      nextArrivalTime = Inf;
    endif

    busyFreeTimes = doctorFreeAt(doctorFreeAt > currentClock);
    if ~isempty(busyFreeTimes)
      nextFreeTime = min(busyFreeTimes);
    else
      nextFreeTime = Inf;
    endif

    nextEventTime = min(nextArrivalTime, nextFreeTime);
    if nextEventTime == Inf
      overflow(~served) = true;
      break;
    endif

    currentClock = nextEventTime;
    if currentClock >= closeTime
      overflow(~served) = true;
      break;
    endif

    while nextArrivalPtr <= n && sortedArrival(nextArrivalPtr) <= currentClock
      waitingQueue(end+1) = order(nextArrivalPtr);
      nextArrivalPtr = nextArrivalPtr + 1;
    endwhile

    range = eligibleSlots(currentClock, improve);
    busyCount = sum(doctorFreeAt(range) > currentClock);
    cap = length(range);

    while busyCount < cap && ~isempty(waitingQueue)
      [~, qPos] = min(priority(waitingQueue));
      patientIdx = waitingQueue(qPos);
      waitingQueue(qPos) = [];

      freeSlotsInRange = range(doctorFreeAt(range) <= currentClock);
      slotIdx = freeSlotsInRange(1);

      startTime(patientIdx)      = currentClock;
      finishTime(patientIdx)     = currentClock + serviceTimes(patientIdx);
      doctorAssigned(patientIdx) = slotIdx;
      doctorFreeAt(slotIdx)      = finishTime(patientIdx);
      served(patientIdx) = true;

      busyCount = busyCount + 1;
    endwhile
  endwhile

  waitTime = startTime - arrivalTimes;
endfunction
