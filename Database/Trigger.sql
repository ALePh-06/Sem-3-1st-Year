--Log when user table is altered
CREATE TRIGGER audit_user_update
AFTER UPDATE ON Users
FOR EACH ROW
BEGIN
    INSERT INTO AuditLog (Table_Name, Action_Type, Record_ID, Changed_By, Change_Detail)
    VALUES ('Users', 'UPDATE', NEW.User_ID, NEW.Email, CONCAT('Role changed to ', NEW.User_Role));
END;

--Log account stat
CREATE TRIGGER audit_account_status
AFTER UPDATE ON CloudAccount
FOR EACH ROW
BEGIN
    IF OLD.Status <> NEW.Status THEN
        INSERT INTO AuditLog (Table_Name, Action_Type, Record_ID, Changed_By, Change_Detail)
        VALUES ('CloudAccount', 'UPDATE', NEW.Account_ID, NEW.User_ID, CONCAT('Status changed from ', OLD.Status, ' to ', NEW.Status));
    END IF;
END;

--Log Severity level
CREATE TRIGGER audit_vulnerability_severity
AFTER UPDATE ON Vulnerability
FOR EACH ROW
BEGIN
    IF OLD.Severity_Level <> NEW.Severity_Level THEN
        INSERT INTO AuditLog (Table_Name, Action_Type, Record_ID, Changed_By, Change_Detail)
        VALUES ('Vulnerability', 'UPDATE', NEW.Vulnerability_ID, NEW.Provider_ID, CONCAT('Severity changed from ', OLD.Severity_Level, ' to ', NEW.Severity_Level));
    END IF;
END;

--Log report
CREATE TRIGGER audit_report_insert
AFTER INSERT ON Report
FOR EACH ROW
BEGIN
    INSERT INTO AuditLog (Table_Name, Action_Type, Record_ID, Changed_By, Change_Detail)
    VALUES ('Report', 'INSERT', NEW.Report_ID, NULL, CONCAT('New report created with risk summary: ', NEW.Risk_Summary));
END;

--Log remedy
CREATE TRIGGER audit_plan_status
AFTER UPDATE ON RemedialPlan
FOR EACH ROW
BEGIN
    IF OLD.Plan_Status <> NEW.Plan_Status THEN
        INSERT INTO AuditLog (Table_Name, Action_Type, Record_ID, Changed_By, Change_Detail)
        VALUES ('RemedialPlan', 'UPDATE', NEW.Plan_ID, NULL, CONCAT('Plan status changed from ', OLD.Plan_Status, ' to ', NEW.Plan_Status));
    END IF;
END;