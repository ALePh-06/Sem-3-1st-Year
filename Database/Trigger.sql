--Trigger 1: Flag when a Vulnerability is added or updated
DROP TRIGGER IF EXISTS trg_vulnerability_flag ON Vulnerability;
DROP FUNCTION IF EXISTS fn_vulnerability_flag();
 
CREATE OR REPLACE FUNCTION fn_vulnerability_flag()
RETURNS TRIGGER AS $$
BEGIN
    IF (TG_OP = 'INSERT') THEN
        RAISE NOTICE '[ALERT] New vulnerability added: % | Type: % | Severity: %',
            NEW.Vulnerability_ID, NEW.Vulnerability_Type, NEW.Severity_Level;
    ELSIF (TG_OP = 'UPDATE') THEN
        RAISE NOTICE '[ALERT] Vulnerability updated: % | Severity now: % (was: %)',
            NEW.Vulnerability_ID, NEW.Severity_Level, OLD.Severity_Level;
    END IF;
 
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
 
CREATE TRIGGER trg_vulnerability_flag
AFTER INSERT OR UPDATE ON Vulnerability
FOR EACH ROW
EXECUTE PROCEDURE fn_vulnerability_flag();
 

 --Flag when a SecurityScan has Status = 'Open'
 DROP TRIGGER IF EXISTS trg_securityscan_open_flag ON SecurityScan;
DROP FUNCTION IF EXISTS fn_securityscan_open_flag();
 
CREATE OR REPLACE FUNCTION fn_securityscan_open_flag()
RETURNS TRIGGER AS $$
BEGIN
    IF (NEW.Status = 'Open') THEN
        RAISE WARNING '[OPEN SCAN] Scan % on Resource % is OPEN (Vulnerability: %) - needs attention!',
            NEW.Scan_ID, NEW.Resource_ID, NEW.Vulnerability_ID;
    END IF;
 
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
 
CREATE TRIGGER trg_securityscan_open_flag
AFTER INSERT OR UPDATE ON SecurityScan
FOR EACH ROW
EXECUTE PROCEDURE fn_securityscan_open_flag();
 