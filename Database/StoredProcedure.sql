CREATE OR REPLACE PROCEDURE AdminRiskAndProgressDetector()
LANGUAGE plpgsql
AS $$
DECLARE
    v_low_risk INTEGER;
    v_medium_risk INTEGER;
    v_high_risk INTEGER;
    v_critical_risk INTEGER;

    v_completed INTEGER;
    v_in_progress INTEGER;
BEGIN

    SELECT COUNT(*)
    INTO v_low_risk
    FROM Report
    WHERE Risk_Summary ILIKE '%low%';

    SELECT COUNT(*)
    INTO v_medium_risk
    FROM Report
    WHERE Risk_Summary ILIKE '%medium%';

    SELECT COUNT(*)
    INTO v_high_risk
    FROM Report
    WHERE Risk_Summary ILIKE '%high%';

    SELECT COUNT(*)
    INTO v_critical_risk
    FROM Report
    WHERE Risk_Summary ILIKE '%critical%';

    SELECT COUNT(*)
    INTO v_completed
    FROM ProgressTracker
    WHERE Tracker_Status ILIKE '%complete%';

    SELECT COUNT(*)
    INTO v_in_progress
    FROM ProgressTracker
    WHERE Tracker_Status ILIKE '%active%';

    RAISE NOTICE '===== SECURITY DASHBOARD =====';
    RAISE NOTICE 'Low Risk: %', v_low_risk;
    RAISE NOTICE 'Medium Risk: %', v_medium_risk;
    RAISE NOTICE 'High Risk: %', v_high_risk;
    RAISE NOTICE 'Critical Risk: %', v_critical_risk;
    RAISE NOTICE '';
    RAISE NOTICE '======  TASKS TRACKER  ======';
    RAISE NOTICE 'Completed Tasks: %', v_completed;
    RAISE NOTICE 'In Progress Tasks: %', v_in_progress;

END;
$$;